
#include "KBEvent.h"
#include "KBDebug.h"

KBMap<KBString, KBArray<KBEvent::EventObj>> KBEvent::events_;
KBArray<KBEvent::FiredEvent*>	KBEvent::firedEvents_;
bool KBEvent::isPause_ = false;

KBEvent::KBEvent()
{
}

KBEvent::~KBEvent()
{
}

void KBEvent::clear()
{
	events_.Clear();
	clearFiredEvents();
}

void KBEvent::clearFiredEvents()
{
	while (firedEvents_.Num() > 0)
	{
		FiredEvent* event = firedEvents_.Pop();
		// delete event->args;
		event->args = nullptr;
		delete event;
	}
}

bool KBEvent::registerEvent(const KBString& eventName, const KBString& funcName, std::function<void(std::shared_ptr<UKBEventData>)> func, void* objPtr)
{
	KBArray<EventObj>* eo_array = NULL;
	KBArray<EventObj>* eo_array_find = events_.Find(eventName);

	if (!eo_array_find)
	{
		events_.Add(eventName, KBArray<EventObj>());
		eo_array = &(*events_.Find(eventName));
	}
	else
	{
		eo_array = &(*eo_array_find);
	}

	EventObj eo;
	eo.funcName = funcName;
	eo.method = func;
	eo.objPtr = objPtr;
	eo_array->Add(eo);
	return true;
}

bool KBEvent::deregister(void* objPtr, const KBString& eventName, const KBString& funcName)
{
	KBArray<EventObj>* eo_array_find = events_.Find(eventName);
	if (!eo_array_find || (*eo_array_find).Num() == 0)
	{
		return false;
	}

	// 从后往前遍历，以避免中途删除的问题
	// for (int i = (*eo_array_find).Num() - 1; i >= 0; --i)
	// {
	// 	EventObj& item = (*eo_array_find)[i];
	// 	if (objPtr == item.objPtr && (funcName.empty() || funcName == item.funcName))
	// 	{
	// 		(*eo_array_find).RemoveAt(i, 1);
	// 	}
	// }

	for (size_t i = eo_array_find->Num(); i-- > 0; )
	{
		EventObj& item = (*eo_array_find)[i];
		if (objPtr == item.objPtr && (funcName.empty() || funcName == item.funcName))
		{
			eo_array_find->RemoveAt(i, 1);
		}
	}

	removeFiredEvent(objPtr, eventName, funcName);

	return true;
}

bool KBEvent::deregister(void* objPtr)
{
	for (auto& item : events_)
	{
		deregister(objPtr, item.first, KBTEXT(""));
	}

	return true;
}

void KBEvent::fire(const KBString& eventName, std::shared_ptr<UKBEventData> pEventData)
{
	if (!pEventData)
	{
		pEventData = std::make_shared<UKBEventData>();
	}

	KBArray<EventObj>* eo_array_find = events_.Find(eventName);
	if (!eo_array_find || (*eo_array_find).Num() == 0)
	{
		//SCREEN_WARNING_MSG("KBEvent::fire(): event(%s) not found!", *eventName);
		return;
	}

	pEventData->eventName = eventName;

	for (auto& item : (*eo_array_find))
	{
		if (!isPause_)
		{
			item.method(pEventData);
			// pEventData->ConditionalBeginDestroy();
			// delete pEventData;
		} 
		else
		{
			FiredEvent* event = new FiredEvent;
			event->evt = item;
			event->eventName = eventName;
			event->args = pEventData;
			firedEvents_.Emplace(event);
		}
	}

	//GetWorld()->ForceGarbageCollection(true);
}

void KBEvent::pause()
{
	isPause_ = true;
}

void KBEvent::resume()
{
	isPause_ = false;
	while (firedEvents_.Num() > 0)
	{
		FiredEvent* event = firedEvents_.Pop();
		event->evt.method(event->args);
		// event->args->ConditionalBeginDestroy();
		// delete event->args;
		event->args = nullptr;
		delete event;
	}
}

void KBEvent::removeFiredEvent(void* objPtr, const KBString& eventName /*= KBTEXT("")*/, const KBString& funcName /*= KBTEXT("")*/)
{
	while (true)
	{
		bool found = false;
		for (auto item : firedEvents_)
		{
			bool ret = (eventName.length() == 0 && funcName.length() == 0) || (item->eventName == eventName && (funcName.length() == 0 || item->evt.funcName == funcName));
			if (ret && item->evt.objPtr == objPtr)
			{
				firedEvents_.Remove(item);
				// item->args->ConditionalBeginDestroy();
				// delete item->args;
				item->args = nullptr;
				delete item;
				found = true;
				break;
			}
		}

		if (!found)
			break;
	}
}
