#include "EntityDef.h"
#include "DataTypes.h"
#include "CustomDataTypes.h"
#include "ScriptModule.h"
#include "Property.h"
#include "Method.h"
#include "Entity.h"

#include "EntityFactory.h"


namespace KBEngine
{

KBMap<KBString, uint16> EntityDef::datatype2id;
KBMap<KBString, DATATYPE_BASE*> EntityDef::datatypes;
KBMap<uint16, DATATYPE_BASE*> EntityDef::id2datatypes;
KBMap<KBString, int32> EntityDef::entityclass;
KBMap<KBString, ScriptModule*> EntityDef::moduledefs;
KBMap<uint16, ScriptModule*> EntityDef::idmoduledefs;

bool EntityDef::initialize()
{
	initDataTypes();
	initDefTypes();
	initScriptModules();
	return true;
}

bool EntityDef::reset()
{
	clear();
	return initialize();
}

void EntityDef::clear()
{
	KBArray<DATATYPE_BASE*> deleted_datatypes;
	for (auto& item : EntityDef::datatypes)
	{
		int32 idx = deleted_datatypes.Find(item.second);
		if (idx != -1)
			continue;

		deleted_datatypes.Add(item.second);
		delete item.second;
	}

	for (auto& item : EntityDef::moduledefs)
		delete item.second;

	datatype2id.Clear();
	datatypes.Clear();
	id2datatypes.Clear();
	entityclass.Clear();
	moduledefs.Clear();
	idmoduledefs.Clear();
}

void EntityDef::initDataTypes()
{
	datatypes.Add(KBTEXT("UINT8"), new DATATYPE_UINT8());
	datatypes.Add(KBTEXT("UINT16"), new DATATYPE_UINT16());
	datatypes.Add(KBTEXT("UINT32"), new DATATYPE_UINT32());
	datatypes.Add(KBTEXT("UINT64"), new DATATYPE_UINT64());

	datatypes.Add(KBTEXT("INT8"), new DATATYPE_INT8());
	datatypes.Add(KBTEXT("INT16"), new DATATYPE_INT16());
	datatypes.Add(KBTEXT("INT32"), new DATATYPE_INT32());
	datatypes.Add(KBTEXT("INT64"), new DATATYPE_INT64());

	datatypes.Add(KBTEXT("FLOAT"), new DATATYPE_FLOAT());
	datatypes.Add(KBTEXT("DOUBLE"), new DATATYPE_DOUBLE());

	datatypes.Add(KBTEXT("STRING"), new DATATYPE_STRING());
	datatypes.Add(KBTEXT("VECTOR2"), new DATATYPE_VECTOR2());

	datatypes.Add(KBTEXT("VECTOR3"), new DATATYPE_VECTOR3());

	datatypes.Add(KBTEXT("VECTOR4"), new DATATYPE_VECTOR4());
	datatypes.Add(KBTEXT("PYTHON"), new DATATYPE_PYTHON());

	datatypes.Add(KBTEXT("UNICODE"), new DATATYPE_UNICODE());
	datatypes.Add(KBTEXT("ENTITYCALL"), new DATATYPE_ENTITYCALL());

	datatypes.Add(KBTEXT("BLOB"), new DATATYPE_BLOB());
}

Entity* EntityDef::createEntity(int utype)
{
	Entity* pEntity = NULL;

	switch(utype)
	{
		case 1:
			pEntity = EntityFactory::instance().create("Account");
			break;
		case 2:
			pEntity = EntityFactory::instance().create("Avatar");
			break;
		case 5:
			pEntity = EntityFactory::instance().create("Monster");
			break;
		case 6:
			pEntity = EntityFactory::instance().create("NPC");
			break;
		case 7:
			pEntity = EntityFactory::instance().create("Gate");
			break;
		default:
			SCREEN_ERROR_MSG("EntityDef::createEntity() : entity(%d) not found!", utype);
			break;
	};

	return pEntity;
}

void EntityDef::initScriptModules()
{
	ScriptModule* pAccountModule = new ScriptModule("Account", 1);
	EntityDef::moduledefs.Add(KBTEXT("Account"), pAccountModule);
	EntityDef::idmoduledefs.Add(1, pAccountModule);

	Property* pAccount_position = new Property();
	pAccount_position->name = KBTEXT("position");
	pAccount_position->properUtype = 40000;
	pAccount_position->properFlags = 4;
	pAccount_position->aliasID = 1;
	KBVariant* pAccount_position_defval = new KBVariant(KBVector3f());
	pAccount_position->pDefaultVal = pAccount_position_defval;
	pAccountModule->propertys.Add(KBTEXT("position"), pAccount_position); 

	pAccountModule->usePropertyDescrAlias = true;
	pAccountModule->idpropertys.Add((uint16)pAccount_position->aliasID, pAccount_position);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Account), property(position / 40000).");

	Property* pAccount_direction = new Property();
	pAccount_direction->name = KBTEXT("direction");
	pAccount_direction->properUtype = 40001;
	pAccount_direction->properFlags = 4;
	pAccount_direction->aliasID = 2;
	KBVariant* pAccount_direction_defval = new KBVariant(KBVector3f());
	pAccount_direction->pDefaultVal = pAccount_direction_defval;
	pAccountModule->propertys.Add(KBTEXT("direction"), pAccount_direction); 

	pAccountModule->usePropertyDescrAlias = true;
	pAccountModule->idpropertys.Add((uint16)pAccount_direction->aliasID, pAccount_direction);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Account), property(direction / 40001).");

	Property* pAccount_spaceID = new Property();
	pAccount_spaceID->name = KBTEXT("spaceID");
	pAccount_spaceID->properUtype = 40002;
	pAccount_spaceID->properFlags = 16;
	pAccount_spaceID->aliasID = 3;
	KBVariant* pAccount_spaceID_defval = new KBVariant((uint32)safe_atoi64(KBTEXT("")));
	pAccount_spaceID->pDefaultVal = pAccount_spaceID_defval;
	pAccountModule->propertys.Add(KBTEXT("spaceID"), pAccount_spaceID); 

	pAccountModule->usePropertyDescrAlias = true;
	pAccountModule->idpropertys.Add((uint16)pAccount_spaceID->aliasID, pAccount_spaceID);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Account), property(spaceID / 40002).");

	Property* pAccount_lastSelCharacter = new Property();
	pAccount_lastSelCharacter->name = KBTEXT("lastSelCharacter");
	pAccount_lastSelCharacter->properUtype = 2;
	pAccount_lastSelCharacter->properFlags = 32;
	pAccount_lastSelCharacter->aliasID = 4;
	KBVariant* pAccount_lastSelCharacter_defval = new KBVariant((uint64)safe_atoi64(KBTEXT("0")));
	pAccount_lastSelCharacter->pDefaultVal = pAccount_lastSelCharacter_defval;
	pAccountModule->propertys.Add(KBTEXT("lastSelCharacter"), pAccount_lastSelCharacter); 

	pAccountModule->usePropertyDescrAlias = true;
	pAccountModule->idpropertys.Add((uint16)pAccount_lastSelCharacter->aliasID, pAccount_lastSelCharacter);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Account), property(lastSelCharacter / 2).");

	KBArray<DATATYPE_BASE*> Account_onCreateAvatarResult_args;
	Account_onCreateAvatarResult_args.Add(EntityDef::id2datatypes[2]);
	Account_onCreateAvatarResult_args.Add(EntityDef::id2datatypes[25]);

	Method* pAccount_onCreateAvatarResult = new Method();
	pAccount_onCreateAvatarResult->name = KBTEXT("onCreateAvatarResult");
	pAccount_onCreateAvatarResult->methodUtype = 10005;
	pAccount_onCreateAvatarResult->aliasID = 1;
	pAccount_onCreateAvatarResult->args = Account_onCreateAvatarResult_args;

	pAccountModule->methods.Add(KBTEXT("onCreateAvatarResult"), pAccount_onCreateAvatarResult); 
	pAccountModule->useMethodDescrAlias = true;
	pAccountModule->idmethods.Add((uint16)pAccount_onCreateAvatarResult->aliasID, pAccount_onCreateAvatarResult);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Account), method(onCreateAvatarResult / 10005).");

	KBArray<DATATYPE_BASE*> Account_onRemoveAvatar_args;
	Account_onRemoveAvatar_args.Add(EntityDef::id2datatypes[5]);

	Method* pAccount_onRemoveAvatar = new Method();
	pAccount_onRemoveAvatar->name = KBTEXT("onRemoveAvatar");
	pAccount_onRemoveAvatar->methodUtype = 3;
	pAccount_onRemoveAvatar->aliasID = 2;
	pAccount_onRemoveAvatar->args = Account_onRemoveAvatar_args;

	pAccountModule->methods.Add(KBTEXT("onRemoveAvatar"), pAccount_onRemoveAvatar); 
	pAccountModule->useMethodDescrAlias = true;
	pAccountModule->idmethods.Add((uint16)pAccount_onRemoveAvatar->aliasID, pAccount_onRemoveAvatar);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Account), method(onRemoveAvatar / 3).");

	KBArray<DATATYPE_BASE*> Account_onReqAvatarList_args;
	Account_onReqAvatarList_args.Add(EntityDef::id2datatypes[26]);

	Method* pAccount_onReqAvatarList = new Method();
	pAccount_onReqAvatarList->name = KBTEXT("onReqAvatarList");
	pAccount_onReqAvatarList->methodUtype = 10003;
	pAccount_onReqAvatarList->aliasID = 3;
	pAccount_onReqAvatarList->args = Account_onReqAvatarList_args;

	pAccountModule->methods.Add(KBTEXT("onReqAvatarList"), pAccount_onReqAvatarList); 
	pAccountModule->useMethodDescrAlias = true;
	pAccountModule->idmethods.Add((uint16)pAccount_onReqAvatarList->aliasID, pAccount_onReqAvatarList);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Account), method(onReqAvatarList / 10003).");

	KBArray<DATATYPE_BASE*> Account_reqAvatarList_args;

	Method* pAccount_reqAvatarList = new Method();
	pAccount_reqAvatarList->name = KBTEXT("reqAvatarList");
	pAccount_reqAvatarList->methodUtype = 10001;
	pAccount_reqAvatarList->aliasID = -1;
	pAccount_reqAvatarList->args = Account_reqAvatarList_args;

	pAccountModule->methods.Add(KBTEXT("reqAvatarList"), pAccount_reqAvatarList); 
	pAccountModule->base_methods.Add(KBTEXT("reqAvatarList"), pAccount_reqAvatarList);

	pAccountModule->idbase_methods.Add(pAccount_reqAvatarList->methodUtype, pAccount_reqAvatarList);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Account), method(reqAvatarList / 10001).");

	KBArray<DATATYPE_BASE*> Account_reqCreateAvatar_args;
	Account_reqCreateAvatar_args.Add(EntityDef::id2datatypes[2]);
	Account_reqCreateAvatar_args.Add(EntityDef::id2datatypes[12]);

	Method* pAccount_reqCreateAvatar = new Method();
	pAccount_reqCreateAvatar->name = KBTEXT("reqCreateAvatar");
	pAccount_reqCreateAvatar->methodUtype = 10002;
	pAccount_reqCreateAvatar->aliasID = -1;
	pAccount_reqCreateAvatar->args = Account_reqCreateAvatar_args;

	pAccountModule->methods.Add(KBTEXT("reqCreateAvatar"), pAccount_reqCreateAvatar); 
	pAccountModule->base_methods.Add(KBTEXT("reqCreateAvatar"), pAccount_reqCreateAvatar);

	pAccountModule->idbase_methods.Add(pAccount_reqCreateAvatar->methodUtype, pAccount_reqCreateAvatar);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Account), method(reqCreateAvatar / 10002).");

	KBArray<DATATYPE_BASE*> Account_reqRemoveAvatar_args;
	Account_reqRemoveAvatar_args.Add(EntityDef::id2datatypes[12]);

	Method* pAccount_reqRemoveAvatar = new Method();
	pAccount_reqRemoveAvatar->name = KBTEXT("reqRemoveAvatar");
	pAccount_reqRemoveAvatar->methodUtype = 1;
	pAccount_reqRemoveAvatar->aliasID = -1;
	pAccount_reqRemoveAvatar->args = Account_reqRemoveAvatar_args;

	pAccountModule->methods.Add(KBTEXT("reqRemoveAvatar"), pAccount_reqRemoveAvatar); 
	pAccountModule->base_methods.Add(KBTEXT("reqRemoveAvatar"), pAccount_reqRemoveAvatar);

	pAccountModule->idbase_methods.Add(pAccount_reqRemoveAvatar->methodUtype, pAccount_reqRemoveAvatar);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Account), method(reqRemoveAvatar / 1).");

	KBArray<DATATYPE_BASE*> Account_reqRemoveAvatarDBID_args;
	Account_reqRemoveAvatarDBID_args.Add(EntityDef::id2datatypes[5]);

	Method* pAccount_reqRemoveAvatarDBID = new Method();
	pAccount_reqRemoveAvatarDBID->name = KBTEXT("reqRemoveAvatarDBID");
	pAccount_reqRemoveAvatarDBID->methodUtype = 2;
	pAccount_reqRemoveAvatarDBID->aliasID = -1;
	pAccount_reqRemoveAvatarDBID->args = Account_reqRemoveAvatarDBID_args;

	pAccountModule->methods.Add(KBTEXT("reqRemoveAvatarDBID"), pAccount_reqRemoveAvatarDBID); 
	pAccountModule->base_methods.Add(KBTEXT("reqRemoveAvatarDBID"), pAccount_reqRemoveAvatarDBID);

	pAccountModule->idbase_methods.Add(pAccount_reqRemoveAvatarDBID->methodUtype, pAccount_reqRemoveAvatarDBID);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Account), method(reqRemoveAvatarDBID / 2).");

	KBArray<DATATYPE_BASE*> Account_selectAvatarGame_args;
	Account_selectAvatarGame_args.Add(EntityDef::id2datatypes[5]);

	Method* pAccount_selectAvatarGame = new Method();
	pAccount_selectAvatarGame->name = KBTEXT("selectAvatarGame");
	pAccount_selectAvatarGame->methodUtype = 10004;
	pAccount_selectAvatarGame->aliasID = -1;
	pAccount_selectAvatarGame->args = Account_selectAvatarGame_args;

	pAccountModule->methods.Add(KBTEXT("selectAvatarGame"), pAccount_selectAvatarGame); 
	pAccountModule->base_methods.Add(KBTEXT("selectAvatarGame"), pAccount_selectAvatarGame);

	pAccountModule->idbase_methods.Add(pAccount_selectAvatarGame->methodUtype, pAccount_selectAvatarGame);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Account), method(selectAvatarGame / 10004).");

	ScriptModule* pAvatarModule = new ScriptModule("Avatar", 2);
	EntityDef::moduledefs.Add(KBTEXT("Avatar"), pAvatarModule);
	EntityDef::idmoduledefs.Add(2, pAvatarModule);

	Property* pAvatar_position = new Property();
	pAvatar_position->name = KBTEXT("position");
	pAvatar_position->properUtype = 40000;
	pAvatar_position->properFlags = 4;
	pAvatar_position->aliasID = 1;
	KBVariant* pAvatar_position_defval = new KBVariant(KBVector3f());
	pAvatar_position->pDefaultVal = pAvatar_position_defval;
	pAvatarModule->propertys.Add(KBTEXT("position"), pAvatar_position); 

	pAvatarModule->usePropertyDescrAlias = true;
	pAvatarModule->idpropertys.Add((uint16)pAvatar_position->aliasID, pAvatar_position);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Avatar), property(position / 40000).");

	Property* pAvatar_direction = new Property();
	pAvatar_direction->name = KBTEXT("direction");
	pAvatar_direction->properUtype = 40001;
	pAvatar_direction->properFlags = 4;
	pAvatar_direction->aliasID = 2;
	KBVariant* pAvatar_direction_defval = new KBVariant(KBVector3f());
	pAvatar_direction->pDefaultVal = pAvatar_direction_defval;
	pAvatarModule->propertys.Add(KBTEXT("direction"), pAvatar_direction); 

	pAvatarModule->usePropertyDescrAlias = true;
	pAvatarModule->idpropertys.Add((uint16)pAvatar_direction->aliasID, pAvatar_direction);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Avatar), property(direction / 40001).");

	Property* pAvatar_spaceID = new Property();
	pAvatar_spaceID->name = KBTEXT("spaceID");
	pAvatar_spaceID->properUtype = 40002;
	pAvatar_spaceID->properFlags = 16;
	pAvatar_spaceID->aliasID = 3;
	KBVariant* pAvatar_spaceID_defval = new KBVariant((uint32)safe_atoi64(KBTEXT("")));
	pAvatar_spaceID->pDefaultVal = pAvatar_spaceID_defval;
	pAvatarModule->propertys.Add(KBTEXT("spaceID"), pAvatar_spaceID); 

	pAvatarModule->usePropertyDescrAlias = true;
	pAvatarModule->idpropertys.Add((uint16)pAvatar_spaceID->aliasID, pAvatar_spaceID);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Avatar), property(spaceID / 40002).");

	Property* pAvatar_HP = new Property();
	pAvatar_HP->name = KBTEXT("HP");
	pAvatar_HP->properUtype = 47001;
	pAvatar_HP->properFlags = 4;
	pAvatar_HP->aliasID = 4;
	KBVariant* pAvatar_HP_defval = new KBVariant((int32)safe_atoi64(KBTEXT("0")));
	pAvatar_HP->pDefaultVal = pAvatar_HP_defval;
	pAvatarModule->propertys.Add(KBTEXT("HP"), pAvatar_HP); 

	pAvatarModule->usePropertyDescrAlias = true;
	pAvatarModule->idpropertys.Add((uint16)pAvatar_HP->aliasID, pAvatar_HP);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Avatar), property(HP / 47001).");

	Property* pAvatar_HP_Max = new Property();
	pAvatar_HP_Max->name = KBTEXT("HP_Max");
	pAvatar_HP_Max->properUtype = 47002;
	pAvatar_HP_Max->properFlags = 4;
	pAvatar_HP_Max->aliasID = 5;
	KBVariant* pAvatar_HP_Max_defval = new KBVariant((int32)safe_atoi64(KBTEXT("0")));
	pAvatar_HP_Max->pDefaultVal = pAvatar_HP_Max_defval;
	pAvatarModule->propertys.Add(KBTEXT("HP_Max"), pAvatar_HP_Max); 

	pAvatarModule->usePropertyDescrAlias = true;
	pAvatarModule->idpropertys.Add((uint16)pAvatar_HP_Max->aliasID, pAvatar_HP_Max);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Avatar), property(HP_Max / 47002).");

	Property* pAvatar_MP = new Property();
	pAvatar_MP->name = KBTEXT("MP");
	pAvatar_MP->properUtype = 47003;
	pAvatar_MP->properFlags = 4;
	pAvatar_MP->aliasID = 6;
	KBVariant* pAvatar_MP_defval = new KBVariant((int32)safe_atoi64(KBTEXT("0")));
	pAvatar_MP->pDefaultVal = pAvatar_MP_defval;
	pAvatarModule->propertys.Add(KBTEXT("MP"), pAvatar_MP); 

	pAvatarModule->usePropertyDescrAlias = true;
	pAvatarModule->idpropertys.Add((uint16)pAvatar_MP->aliasID, pAvatar_MP);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Avatar), property(MP / 47003).");

	Property* pAvatar_MP_Max = new Property();
	pAvatar_MP_Max->name = KBTEXT("MP_Max");
	pAvatar_MP_Max->properUtype = 47004;
	pAvatar_MP_Max->properFlags = 4;
	pAvatar_MP_Max->aliasID = 7;
	KBVariant* pAvatar_MP_Max_defval = new KBVariant((int32)safe_atoi64(KBTEXT("0")));
	pAvatar_MP_Max->pDefaultVal = pAvatar_MP_Max_defval;
	pAvatarModule->propertys.Add(KBTEXT("MP_Max"), pAvatar_MP_Max); 

	pAvatarModule->usePropertyDescrAlias = true;
	pAvatarModule->idpropertys.Add((uint16)pAvatar_MP_Max->aliasID, pAvatar_MP_Max);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Avatar), property(MP_Max / 47004).");

	Property* pAvatar_component1 = new Property();
	pAvatar_component1->name = KBTEXT("component1");
	pAvatar_component1->properUtype = 16;
	pAvatar_component1->properFlags = 253;
	pAvatar_component1->aliasID = 8;
	pAvatarModule->propertys.Add(KBTEXT("component1"), pAvatar_component1); 

	pAvatarModule->usePropertyDescrAlias = true;
	pAvatarModule->idpropertys.Add((uint16)pAvatar_component1->aliasID, pAvatar_component1);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Avatar), property(component1 / 16).");

	Property* pAvatar_component2 = new Property();
	pAvatar_component2->name = KBTEXT("component2");
	pAvatar_component2->properUtype = 21;
	pAvatar_component2->properFlags = 97;
	pAvatar_component2->aliasID = 9;
	pAvatarModule->propertys.Add(KBTEXT("component2"), pAvatar_component2); 

	pAvatarModule->usePropertyDescrAlias = true;
	pAvatarModule->idpropertys.Add((uint16)pAvatar_component2->aliasID, pAvatar_component2);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Avatar), property(component2 / 21).");

	Property* pAvatar_component3 = new Property();
	pAvatar_component3->name = KBTEXT("component3");
	pAvatar_component3->properUtype = 22;
	pAvatar_component3->properFlags = 157;
	pAvatar_component3->aliasID = 10;
	pAvatarModule->propertys.Add(KBTEXT("component3"), pAvatar_component3); 

	pAvatarModule->usePropertyDescrAlias = true;
	pAvatarModule->idpropertys.Add((uint16)pAvatar_component3->aliasID, pAvatar_component3);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Avatar), property(component3 / 22).");

	Property* pAvatar_forbids = new Property();
	pAvatar_forbids->name = KBTEXT("forbids");
	pAvatar_forbids->properUtype = 47005;
	pAvatar_forbids->properFlags = 4;
	pAvatar_forbids->aliasID = 11;
	KBVariant* pAvatar_forbids_defval = new KBVariant((int32)safe_atoi64(KBTEXT("0")));
	pAvatar_forbids->pDefaultVal = pAvatar_forbids_defval;
	pAvatarModule->propertys.Add(KBTEXT("forbids"), pAvatar_forbids); 

	pAvatarModule->usePropertyDescrAlias = true;
	pAvatarModule->idpropertys.Add((uint16)pAvatar_forbids->aliasID, pAvatar_forbids);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Avatar), property(forbids / 47005).");

	Property* pAvatar_level = new Property();
	pAvatar_level->name = KBTEXT("level");
	pAvatar_level->properUtype = 41002;
	pAvatar_level->properFlags = 8;
	pAvatar_level->aliasID = 12;
	KBVariant* pAvatar_level_defval = new KBVariant((uint16)safe_atoi64(KBTEXT("")));
	pAvatar_level->pDefaultVal = pAvatar_level_defval;
	pAvatarModule->propertys.Add(KBTEXT("level"), pAvatar_level); 

	pAvatarModule->usePropertyDescrAlias = true;
	pAvatarModule->idpropertys.Add((uint16)pAvatar_level->aliasID, pAvatar_level);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Avatar), property(level / 41002).");

	Property* pAvatar_modelID = new Property();
	pAvatar_modelID->name = KBTEXT("modelID");
	pAvatar_modelID->properUtype = 41006;
	pAvatar_modelID->properFlags = 4;
	pAvatar_modelID->aliasID = 13;
	KBVariant* pAvatar_modelID_defval = new KBVariant((uint32)safe_atoi64(KBTEXT("0")));
	pAvatar_modelID->pDefaultVal = pAvatar_modelID_defval;
	pAvatarModule->propertys.Add(KBTEXT("modelID"), pAvatar_modelID); 

	pAvatarModule->usePropertyDescrAlias = true;
	pAvatarModule->idpropertys.Add((uint16)pAvatar_modelID->aliasID, pAvatar_modelID);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Avatar), property(modelID / 41006).");

	Property* pAvatar_modelScale = new Property();
	pAvatar_modelScale->name = KBTEXT("modelScale");
	pAvatar_modelScale->properUtype = 41007;
	pAvatar_modelScale->properFlags = 4;
	pAvatar_modelScale->aliasID = 14;
	KBVariant* pAvatar_modelScale_defval = new KBVariant((uint8)safe_atoi64(KBTEXT("30")));
	pAvatar_modelScale->pDefaultVal = pAvatar_modelScale_defval;
	pAvatarModule->propertys.Add(KBTEXT("modelScale"), pAvatar_modelScale); 

	pAvatarModule->usePropertyDescrAlias = true;
	pAvatarModule->idpropertys.Add((uint16)pAvatar_modelScale->aliasID, pAvatar_modelScale);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Avatar), property(modelScale / 41007).");

	Property* pAvatar_moveSpeed = new Property();
	pAvatar_moveSpeed->name = KBTEXT("moveSpeed");
	pAvatar_moveSpeed->properUtype = 11;
	pAvatar_moveSpeed->properFlags = 4;
	pAvatar_moveSpeed->aliasID = 15;
	KBVariant* pAvatar_moveSpeed_defval = new KBVariant((uint8)safe_atoi64(KBTEXT("50")));
	pAvatar_moveSpeed->pDefaultVal = pAvatar_moveSpeed_defval;
	pAvatarModule->propertys.Add(KBTEXT("moveSpeed"), pAvatar_moveSpeed); 

	pAvatarModule->usePropertyDescrAlias = true;
	pAvatarModule->idpropertys.Add((uint16)pAvatar_moveSpeed->aliasID, pAvatar_moveSpeed);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Avatar), property(moveSpeed / 11).");

	Property* pAvatar_name = new Property();
	pAvatar_name->name = KBTEXT("name");
	pAvatar_name->properUtype = 41003;
	pAvatar_name->properFlags = 4;
	pAvatar_name->aliasID = 16;
	KBVariant* pAvatar_name_defval = new KBVariant(KBString());
	pAvatar_name->pDefaultVal = pAvatar_name_defval;
	pAvatarModule->propertys.Add(KBTEXT("name"), pAvatar_name); 

	pAvatarModule->usePropertyDescrAlias = true;
	pAvatarModule->idpropertys.Add((uint16)pAvatar_name->aliasID, pAvatar_name);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Avatar), property(name / 41003).");

	Property* pAvatar_own_val = new Property();
	pAvatar_own_val->name = KBTEXT("own_val");
	pAvatar_own_val->properUtype = 6;
	pAvatar_own_val->properFlags = 16;
	pAvatar_own_val->aliasID = 17;
	KBVariant* pAvatar_own_val_defval = new KBVariant((uint16)safe_atoi64(KBTEXT("")));
	pAvatar_own_val->pDefaultVal = pAvatar_own_val_defval;
	pAvatarModule->propertys.Add(KBTEXT("own_val"), pAvatar_own_val); 

	pAvatarModule->usePropertyDescrAlias = true;
	pAvatarModule->idpropertys.Add((uint16)pAvatar_own_val->aliasID, pAvatar_own_val);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Avatar), property(own_val / 6).");

	Property* pAvatar_spaceUType = new Property();
	pAvatar_spaceUType->name = KBTEXT("spaceUType");
	pAvatar_spaceUType->properUtype = 41001;
	pAvatar_spaceUType->properFlags = 8;
	pAvatar_spaceUType->aliasID = 18;
	KBVariant* pAvatar_spaceUType_defval = new KBVariant((uint32)safe_atoi64(KBTEXT("")));
	pAvatar_spaceUType->pDefaultVal = pAvatar_spaceUType_defval;
	pAvatarModule->propertys.Add(KBTEXT("spaceUType"), pAvatar_spaceUType); 

	pAvatarModule->usePropertyDescrAlias = true;
	pAvatarModule->idpropertys.Add((uint16)pAvatar_spaceUType->aliasID, pAvatar_spaceUType);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Avatar), property(spaceUType / 41001).");

	Property* pAvatar_state = new Property();
	pAvatar_state->name = KBTEXT("state");
	pAvatar_state->properUtype = 47006;
	pAvatar_state->properFlags = 4;
	pAvatar_state->aliasID = 19;
	KBVariant* pAvatar_state_defval = new KBVariant((int8)safe_atoi64(KBTEXT("0")));
	pAvatar_state->pDefaultVal = pAvatar_state_defval;
	pAvatarModule->propertys.Add(KBTEXT("state"), pAvatar_state); 

	pAvatarModule->usePropertyDescrAlias = true;
	pAvatarModule->idpropertys.Add((uint16)pAvatar_state->aliasID, pAvatar_state);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Avatar), property(state / 47006).");

	Property* pAvatar_subState = new Property();
	pAvatar_subState->name = KBTEXT("subState");
	pAvatar_subState->properUtype = 47007;
	pAvatar_subState->properFlags = 4;
	pAvatar_subState->aliasID = 20;
	KBVariant* pAvatar_subState_defval = new KBVariant((uint8)safe_atoi64(KBTEXT("")));
	pAvatar_subState->pDefaultVal = pAvatar_subState_defval;
	pAvatarModule->propertys.Add(KBTEXT("subState"), pAvatar_subState); 

	pAvatarModule->usePropertyDescrAlias = true;
	pAvatarModule->idpropertys.Add((uint16)pAvatar_subState->aliasID, pAvatar_subState);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Avatar), property(subState / 47007).");

	Property* pAvatar_uid = new Property();
	pAvatar_uid->name = KBTEXT("uid");
	pAvatar_uid->properUtype = 41004;
	pAvatar_uid->properFlags = 4;
	pAvatar_uid->aliasID = 21;
	KBVariant* pAvatar_uid_defval = new KBVariant((uint32)safe_atoi64(KBTEXT("0")));
	pAvatar_uid->pDefaultVal = pAvatar_uid_defval;
	pAvatarModule->propertys.Add(KBTEXT("uid"), pAvatar_uid); 

	pAvatarModule->usePropertyDescrAlias = true;
	pAvatarModule->idpropertys.Add((uint16)pAvatar_uid->aliasID, pAvatar_uid);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Avatar), property(uid / 41004).");

	Property* pAvatar_utype = new Property();
	pAvatar_utype->name = KBTEXT("utype");
	pAvatar_utype->properUtype = 41005;
	pAvatar_utype->properFlags = 4;
	pAvatar_utype->aliasID = 22;
	KBVariant* pAvatar_utype_defval = new KBVariant((uint32)safe_atoi64(KBTEXT("0")));
	pAvatar_utype->pDefaultVal = pAvatar_utype_defval;
	pAvatarModule->propertys.Add(KBTEXT("utype"), pAvatar_utype); 

	pAvatarModule->usePropertyDescrAlias = true;
	pAvatarModule->idpropertys.Add((uint16)pAvatar_utype->aliasID, pAvatar_utype);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Avatar), property(utype / 41005).");

	KBArray<DATATYPE_BASE*> Avatar_dialog_addOption_args;
	Avatar_dialog_addOption_args.Add(EntityDef::id2datatypes[2]);
	Avatar_dialog_addOption_args.Add(EntityDef::id2datatypes[4]);
	Avatar_dialog_addOption_args.Add(EntityDef::id2datatypes[12]);
	Avatar_dialog_addOption_args.Add(EntityDef::id2datatypes[8]);

	Method* pAvatar_dialog_addOption = new Method();
	pAvatar_dialog_addOption->name = KBTEXT("dialog_addOption");
	pAvatar_dialog_addOption->methodUtype = 10101;
	pAvatar_dialog_addOption->aliasID = 1;
	pAvatar_dialog_addOption->args = Avatar_dialog_addOption_args;

	pAvatarModule->methods.Add(KBTEXT("dialog_addOption"), pAvatar_dialog_addOption); 
	pAvatarModule->useMethodDescrAlias = true;
	pAvatarModule->idmethods.Add((uint16)pAvatar_dialog_addOption->aliasID, pAvatar_dialog_addOption);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Avatar), method(dialog_addOption / 10101).");

	KBArray<DATATYPE_BASE*> Avatar_dialog_close_args;

	Method* pAvatar_dialog_close = new Method();
	pAvatar_dialog_close->name = KBTEXT("dialog_close");
	pAvatar_dialog_close->methodUtype = 10104;
	pAvatar_dialog_close->aliasID = 2;
	pAvatar_dialog_close->args = Avatar_dialog_close_args;

	pAvatarModule->methods.Add(KBTEXT("dialog_close"), pAvatar_dialog_close); 
	pAvatarModule->useMethodDescrAlias = true;
	pAvatarModule->idmethods.Add((uint16)pAvatar_dialog_close->aliasID, pAvatar_dialog_close);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Avatar), method(dialog_close / 10104).");

	KBArray<DATATYPE_BASE*> Avatar_dialog_setText_args;
	Avatar_dialog_setText_args.Add(EntityDef::id2datatypes[12]);
	Avatar_dialog_setText_args.Add(EntityDef::id2datatypes[2]);
	Avatar_dialog_setText_args.Add(EntityDef::id2datatypes[4]);
	Avatar_dialog_setText_args.Add(EntityDef::id2datatypes[12]);

	Method* pAvatar_dialog_setText = new Method();
	pAvatar_dialog_setText->name = KBTEXT("dialog_setText");
	pAvatar_dialog_setText->methodUtype = 10102;
	pAvatar_dialog_setText->aliasID = 3;
	pAvatar_dialog_setText->args = Avatar_dialog_setText_args;

	pAvatarModule->methods.Add(KBTEXT("dialog_setText"), pAvatar_dialog_setText); 
	pAvatarModule->useMethodDescrAlias = true;
	pAvatarModule->idmethods.Add((uint16)pAvatar_dialog_setText->aliasID, pAvatar_dialog_setText);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Avatar), method(dialog_setText / 10102).");

	KBArray<DATATYPE_BASE*> Avatar_onAddSkill_args;
	Avatar_onAddSkill_args.Add(EntityDef::id2datatypes[8]);

	Method* pAvatar_onAddSkill = new Method();
	pAvatar_onAddSkill->name = KBTEXT("onAddSkill");
	pAvatar_onAddSkill->methodUtype = 12;
	pAvatar_onAddSkill->aliasID = 4;
	pAvatar_onAddSkill->args = Avatar_onAddSkill_args;

	pAvatarModule->methods.Add(KBTEXT("onAddSkill"), pAvatar_onAddSkill); 
	pAvatarModule->useMethodDescrAlias = true;
	pAvatarModule->idmethods.Add((uint16)pAvatar_onAddSkill->aliasID, pAvatar_onAddSkill);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Avatar), method(onAddSkill / 12).");

	KBArray<DATATYPE_BASE*> Avatar_onJump_args;

	Method* pAvatar_onJump = new Method();
	pAvatar_onJump->name = KBTEXT("onJump");
	pAvatar_onJump->methodUtype = 7;
	pAvatar_onJump->aliasID = 5;
	pAvatar_onJump->args = Avatar_onJump_args;

	pAvatarModule->methods.Add(KBTEXT("onJump"), pAvatar_onJump); 
	pAvatarModule->useMethodDescrAlias = true;
	pAvatarModule->idmethods.Add((uint16)pAvatar_onJump->aliasID, pAvatar_onJump);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Avatar), method(onJump / 7).");

	KBArray<DATATYPE_BASE*> Avatar_onRemoveSkill_args;
	Avatar_onRemoveSkill_args.Add(EntityDef::id2datatypes[8]);

	Method* pAvatar_onRemoveSkill = new Method();
	pAvatar_onRemoveSkill->name = KBTEXT("onRemoveSkill");
	pAvatar_onRemoveSkill->methodUtype = 13;
	pAvatar_onRemoveSkill->aliasID = 6;
	pAvatar_onRemoveSkill->args = Avatar_onRemoveSkill_args;

	pAvatarModule->methods.Add(KBTEXT("onRemoveSkill"), pAvatar_onRemoveSkill); 
	pAvatarModule->useMethodDescrAlias = true;
	pAvatarModule->idmethods.Add((uint16)pAvatar_onRemoveSkill->aliasID, pAvatar_onRemoveSkill);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Avatar), method(onRemoveSkill / 13).");

	KBArray<DATATYPE_BASE*> Avatar_recvDamage_args;
	Avatar_recvDamage_args.Add(EntityDef::id2datatypes[8]);
	Avatar_recvDamage_args.Add(EntityDef::id2datatypes[8]);
	Avatar_recvDamage_args.Add(EntityDef::id2datatypes[8]);
	Avatar_recvDamage_args.Add(EntityDef::id2datatypes[8]);

	Method* pAvatar_recvDamage = new Method();
	pAvatar_recvDamage->name = KBTEXT("recvDamage");
	pAvatar_recvDamage->methodUtype = 16;
	pAvatar_recvDamage->aliasID = 7;
	pAvatar_recvDamage->args = Avatar_recvDamage_args;

	pAvatarModule->methods.Add(KBTEXT("recvDamage"), pAvatar_recvDamage); 
	pAvatarModule->useMethodDescrAlias = true;
	pAvatarModule->idmethods.Add((uint16)pAvatar_recvDamage->aliasID, pAvatar_recvDamage);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Avatar), method(recvDamage / 16).");

	KBArray<DATATYPE_BASE*> Avatar_dialog_args;
	Avatar_dialog_args.Add(EntityDef::id2datatypes[8]);
	Avatar_dialog_args.Add(EntityDef::id2datatypes[4]);

	Method* pAvatar_dialog = new Method();
	pAvatar_dialog->name = KBTEXT("dialog");
	pAvatar_dialog->methodUtype = 11003;
	pAvatar_dialog->aliasID = -1;
	pAvatar_dialog->args = Avatar_dialog_args;

	pAvatarModule->methods.Add(KBTEXT("dialog"), pAvatar_dialog); 
	pAvatarModule->cell_methods.Add(KBTEXT("dialog"), pAvatar_dialog);

	pAvatarModule->idcell_methods.Add(pAvatar_dialog->methodUtype, pAvatar_dialog);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Avatar), method(dialog / 11003).");

	KBArray<DATATYPE_BASE*> Avatar_jump_args;

	Method* pAvatar_jump = new Method();
	pAvatar_jump->name = KBTEXT("jump");
	pAvatar_jump->methodUtype = 5;
	pAvatar_jump->aliasID = -1;
	pAvatar_jump->args = Avatar_jump_args;

	pAvatarModule->methods.Add(KBTEXT("jump"), pAvatar_jump); 
	pAvatarModule->cell_methods.Add(KBTEXT("jump"), pAvatar_jump);

	pAvatarModule->idcell_methods.Add(pAvatar_jump->methodUtype, pAvatar_jump);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Avatar), method(jump / 5).");

	KBArray<DATATYPE_BASE*> Avatar_relive_args;
	Avatar_relive_args.Add(EntityDef::id2datatypes[2]);

	Method* pAvatar_relive = new Method();
	pAvatar_relive->name = KBTEXT("relive");
	pAvatar_relive->methodUtype = 4;
	pAvatar_relive->aliasID = -1;
	pAvatar_relive->args = Avatar_relive_args;

	pAvatarModule->methods.Add(KBTEXT("relive"), pAvatar_relive); 
	pAvatarModule->cell_methods.Add(KBTEXT("relive"), pAvatar_relive);

	pAvatarModule->idcell_methods.Add(pAvatar_relive->methodUtype, pAvatar_relive);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Avatar), method(relive / 4).");

	KBArray<DATATYPE_BASE*> Avatar_requestPull_args;

	Method* pAvatar_requestPull = new Method();
	pAvatar_requestPull->name = KBTEXT("requestPull");
	pAvatar_requestPull->methodUtype = 11;
	pAvatar_requestPull->aliasID = -1;
	pAvatar_requestPull->args = Avatar_requestPull_args;

	pAvatarModule->methods.Add(KBTEXT("requestPull"), pAvatar_requestPull); 
	pAvatarModule->cell_methods.Add(KBTEXT("requestPull"), pAvatar_requestPull);

	pAvatarModule->idcell_methods.Add(pAvatar_requestPull->methodUtype, pAvatar_requestPull);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Avatar), method(requestPull / 11).");

	KBArray<DATATYPE_BASE*> Avatar_useTargetSkill_args;
	Avatar_useTargetSkill_args.Add(EntityDef::id2datatypes[8]);
	Avatar_useTargetSkill_args.Add(EntityDef::id2datatypes[8]);

	Method* pAvatar_useTargetSkill = new Method();
	pAvatar_useTargetSkill->name = KBTEXT("useTargetSkill");
	pAvatar_useTargetSkill->methodUtype = 11001;
	pAvatar_useTargetSkill->aliasID = -1;
	pAvatar_useTargetSkill->args = Avatar_useTargetSkill_args;

	pAvatarModule->methods.Add(KBTEXT("useTargetSkill"), pAvatar_useTargetSkill); 
	pAvatarModule->cell_methods.Add(KBTEXT("useTargetSkill"), pAvatar_useTargetSkill);

	pAvatarModule->idcell_methods.Add(pAvatar_useTargetSkill->methodUtype, pAvatar_useTargetSkill);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Avatar), method(useTargetSkill / 11001).");

	ScriptModule* pTestModule = new ScriptModule("Test", 3);
	EntityDef::moduledefs.Add(KBTEXT("Test"), pTestModule);
	EntityDef::idmoduledefs.Add(3, pTestModule);

	Property* pTest_position = new Property();
	pTest_position->name = KBTEXT("position");
	pTest_position->properUtype = 40000;
	pTest_position->properFlags = 4;
	pTest_position->aliasID = 1;
	KBVariant* pTest_position_defval = new KBVariant(KBVector3f());
	pTest_position->pDefaultVal = pTest_position_defval;
	pTestModule->propertys.Add(KBTEXT("position"), pTest_position); 

	pTestModule->usePropertyDescrAlias = true;
	pTestModule->idpropertys.Add((uint16)pTest_position->aliasID, pTest_position);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Test), property(position / 40000).");

	Property* pTest_direction = new Property();
	pTest_direction->name = KBTEXT("direction");
	pTest_direction->properUtype = 40001;
	pTest_direction->properFlags = 4;
	pTest_direction->aliasID = 2;
	KBVariant* pTest_direction_defval = new KBVariant(KBVector3f());
	pTest_direction->pDefaultVal = pTest_direction_defval;
	pTestModule->propertys.Add(KBTEXT("direction"), pTest_direction); 

	pTestModule->usePropertyDescrAlias = true;
	pTestModule->idpropertys.Add((uint16)pTest_direction->aliasID, pTest_direction);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Test), property(direction / 40001).");

	Property* pTest_spaceID = new Property();
	pTest_spaceID->name = KBTEXT("spaceID");
	pTest_spaceID->properUtype = 40002;
	pTest_spaceID->properFlags = 16;
	pTest_spaceID->aliasID = 3;
	KBVariant* pTest_spaceID_defval = new KBVariant((uint32)safe_atoi64(KBTEXT("")));
	pTest_spaceID->pDefaultVal = pTest_spaceID_defval;
	pTestModule->propertys.Add(KBTEXT("spaceID"), pTest_spaceID); 

	pTestModule->usePropertyDescrAlias = true;
	pTestModule->idpropertys.Add((uint16)pTest_spaceID->aliasID, pTest_spaceID);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Test), property(spaceID / 40002).");

	Property* pTest_own = new Property();
	pTest_own->name = KBTEXT("own");
	pTest_own->properUtype = 18;
	pTest_own->properFlags = 8;
	pTest_own->aliasID = 4;
	KBVariant* pTest_own_defval = new KBVariant((int32)safe_atoi64(KBTEXT("1001")));
	pTest_own->pDefaultVal = pTest_own_defval;
	pTestModule->propertys.Add(KBTEXT("own"), pTest_own); 

	pTestModule->usePropertyDescrAlias = true;
	pTestModule->idpropertys.Add((uint16)pTest_own->aliasID, pTest_own);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Test), property(own / 18).");

	Property* pTest_state = new Property();
	pTest_state->name = KBTEXT("state");
	pTest_state->properUtype = 17;
	pTest_state->properFlags = 4;
	pTest_state->aliasID = 5;
	KBVariant* pTest_state_defval = new KBVariant((int32)safe_atoi64(KBTEXT("100")));
	pTest_state->pDefaultVal = pTest_state_defval;
	pTestModule->propertys.Add(KBTEXT("state"), pTest_state); 

	pTestModule->usePropertyDescrAlias = true;
	pTestModule->idpropertys.Add((uint16)pTest_state->aliasID, pTest_state);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Test), property(state / 17).");

	KBArray<DATATYPE_BASE*> Test_helloCB_args;
	Test_helloCB_args.Add(EntityDef::id2datatypes[8]);

	Method* pTest_helloCB = new Method();
	pTest_helloCB->name = KBTEXT("helloCB");
	pTest_helloCB->methodUtype = 28;
	pTest_helloCB->aliasID = 1;
	pTest_helloCB->args = Test_helloCB_args;

	pTestModule->methods.Add(KBTEXT("helloCB"), pTest_helloCB); 
	pTestModule->useMethodDescrAlias = true;
	pTestModule->idmethods.Add((uint16)pTest_helloCB->aliasID, pTest_helloCB);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Test), method(helloCB / 28).");

	KBArray<DATATYPE_BASE*> Test_say_args;
	Test_say_args.Add(EntityDef::id2datatypes[8]);

	Method* pTest_say = new Method();
	pTest_say->name = KBTEXT("say");
	pTest_say->methodUtype = 27;
	pTest_say->aliasID = -1;
	pTest_say->args = Test_say_args;

	pTestModule->methods.Add(KBTEXT("say"), pTest_say); 
	pTestModule->base_methods.Add(KBTEXT("say"), pTest_say);

	pTestModule->idbase_methods.Add(pTest_say->methodUtype, pTest_say);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Test), method(say / 27).");

	KBArray<DATATYPE_BASE*> Test_hello_args;
	Test_hello_args.Add(EntityDef::id2datatypes[8]);

	Method* pTest_hello = new Method();
	pTest_hello->name = KBTEXT("hello");
	pTest_hello->methodUtype = 26;
	pTest_hello->aliasID = -1;
	pTest_hello->args = Test_hello_args;

	pTestModule->methods.Add(KBTEXT("hello"), pTest_hello); 
	pTestModule->cell_methods.Add(KBTEXT("hello"), pTest_hello);

	pTestModule->idcell_methods.Add(pTest_hello->methodUtype, pTest_hello);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Test), method(hello / 26).");

	ScriptModule* pTestNoBaseModule = new ScriptModule("TestNoBase", 4);
	EntityDef::moduledefs.Add(KBTEXT("TestNoBase"), pTestNoBaseModule);
	EntityDef::idmoduledefs.Add(4, pTestNoBaseModule);

	Property* pTestNoBase_position = new Property();
	pTestNoBase_position->name = KBTEXT("position");
	pTestNoBase_position->properUtype = 40000;
	pTestNoBase_position->properFlags = 4;
	pTestNoBase_position->aliasID = 1;
	KBVariant* pTestNoBase_position_defval = new KBVariant(KBVector3f());
	pTestNoBase_position->pDefaultVal = pTestNoBase_position_defval;
	pTestNoBaseModule->propertys.Add(KBTEXT("position"), pTestNoBase_position); 

	pTestNoBaseModule->usePropertyDescrAlias = true;
	pTestNoBaseModule->idpropertys.Add((uint16)pTestNoBase_position->aliasID, pTestNoBase_position);

	//DEBUG_MSG("EntityDef::initScriptModules: add(TestNoBase), property(position / 40000).");

	Property* pTestNoBase_direction = new Property();
	pTestNoBase_direction->name = KBTEXT("direction");
	pTestNoBase_direction->properUtype = 40001;
	pTestNoBase_direction->properFlags = 4;
	pTestNoBase_direction->aliasID = 2;
	KBVariant* pTestNoBase_direction_defval = new KBVariant(KBVector3f());
	pTestNoBase_direction->pDefaultVal = pTestNoBase_direction_defval;
	pTestNoBaseModule->propertys.Add(KBTEXT("direction"), pTestNoBase_direction); 

	pTestNoBaseModule->usePropertyDescrAlias = true;
	pTestNoBaseModule->idpropertys.Add((uint16)pTestNoBase_direction->aliasID, pTestNoBase_direction);

	//DEBUG_MSG("EntityDef::initScriptModules: add(TestNoBase), property(direction / 40001).");

	Property* pTestNoBase_spaceID = new Property();
	pTestNoBase_spaceID->name = KBTEXT("spaceID");
	pTestNoBase_spaceID->properUtype = 40002;
	pTestNoBase_spaceID->properFlags = 16;
	pTestNoBase_spaceID->aliasID = 3;
	KBVariant* pTestNoBase_spaceID_defval = new KBVariant((uint32)safe_atoi64(KBTEXT("")));
	pTestNoBase_spaceID->pDefaultVal = pTestNoBase_spaceID_defval;
	pTestNoBaseModule->propertys.Add(KBTEXT("spaceID"), pTestNoBase_spaceID); 

	pTestNoBaseModule->usePropertyDescrAlias = true;
	pTestNoBaseModule->idpropertys.Add((uint16)pTestNoBase_spaceID->aliasID, pTestNoBase_spaceID);

	//DEBUG_MSG("EntityDef::initScriptModules: add(TestNoBase), property(spaceID / 40002).");

	Property* pTestNoBase_own = new Property();
	pTestNoBase_own->name = KBTEXT("own");
	pTestNoBase_own->properUtype = 24;
	pTestNoBase_own->properFlags = 8;
	pTestNoBase_own->aliasID = 4;
	KBVariant* pTestNoBase_own_defval = new KBVariant((int32)safe_atoi64(KBTEXT("1001")));
	pTestNoBase_own->pDefaultVal = pTestNoBase_own_defval;
	pTestNoBaseModule->propertys.Add(KBTEXT("own"), pTestNoBase_own); 

	pTestNoBaseModule->usePropertyDescrAlias = true;
	pTestNoBaseModule->idpropertys.Add((uint16)pTestNoBase_own->aliasID, pTestNoBase_own);

	//DEBUG_MSG("EntityDef::initScriptModules: add(TestNoBase), property(own / 24).");

	Property* pTestNoBase_state = new Property();
	pTestNoBase_state->name = KBTEXT("state");
	pTestNoBase_state->properUtype = 23;
	pTestNoBase_state->properFlags = 4;
	pTestNoBase_state->aliasID = 5;
	KBVariant* pTestNoBase_state_defval = new KBVariant((int32)safe_atoi64(KBTEXT("100")));
	pTestNoBase_state->pDefaultVal = pTestNoBase_state_defval;
	pTestNoBaseModule->propertys.Add(KBTEXT("state"), pTestNoBase_state); 

	pTestNoBaseModule->usePropertyDescrAlias = true;
	pTestNoBaseModule->idpropertys.Add((uint16)pTestNoBase_state->aliasID, pTestNoBase_state);

	//DEBUG_MSG("EntityDef::initScriptModules: add(TestNoBase), property(state / 23).");

	KBArray<DATATYPE_BASE*> TestNoBase_helloCB_args;
	TestNoBase_helloCB_args.Add(EntityDef::id2datatypes[8]);

	Method* pTestNoBase_helloCB = new Method();
	pTestNoBase_helloCB->name = KBTEXT("helloCB");
	pTestNoBase_helloCB->methodUtype = 30;
	pTestNoBase_helloCB->aliasID = 1;
	pTestNoBase_helloCB->args = TestNoBase_helloCB_args;

	pTestNoBaseModule->methods.Add(KBTEXT("helloCB"), pTestNoBase_helloCB); 
	pTestNoBaseModule->useMethodDescrAlias = true;
	pTestNoBaseModule->idmethods.Add((uint16)pTestNoBase_helloCB->aliasID, pTestNoBase_helloCB);

	//DEBUG_MSG("EntityDef::initScriptModules: add(TestNoBase), method(helloCB / 30).");

	KBArray<DATATYPE_BASE*> TestNoBase_hello_args;
	TestNoBase_hello_args.Add(EntityDef::id2datatypes[8]);

	Method* pTestNoBase_hello = new Method();
	pTestNoBase_hello->name = KBTEXT("hello");
	pTestNoBase_hello->methodUtype = 29;
	pTestNoBase_hello->aliasID = -1;
	pTestNoBase_hello->args = TestNoBase_hello_args;

	pTestNoBaseModule->methods.Add(KBTEXT("hello"), pTestNoBase_hello); 
	pTestNoBaseModule->cell_methods.Add(KBTEXT("hello"), pTestNoBase_hello);

	pTestNoBaseModule->idcell_methods.Add(pTestNoBase_hello->methodUtype, pTestNoBase_hello);

	//DEBUG_MSG("EntityDef::initScriptModules: add(TestNoBase), method(hello / 29).");

	ScriptModule* pMonsterModule = new ScriptModule("Monster", 5);
	EntityDef::moduledefs.Add(KBTEXT("Monster"), pMonsterModule);
	EntityDef::idmoduledefs.Add(5, pMonsterModule);

	Property* pMonster_position = new Property();
	pMonster_position->name = KBTEXT("position");
	pMonster_position->properUtype = 40000;
	pMonster_position->properFlags = 4;
	pMonster_position->aliasID = 1;
	KBVariant* pMonster_position_defval = new KBVariant(KBVector3f());
	pMonster_position->pDefaultVal = pMonster_position_defval;
	pMonsterModule->propertys.Add(KBTEXT("position"), pMonster_position); 

	pMonsterModule->usePropertyDescrAlias = true;
	pMonsterModule->idpropertys.Add((uint16)pMonster_position->aliasID, pMonster_position);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Monster), property(position / 40000).");

	Property* pMonster_direction = new Property();
	pMonster_direction->name = KBTEXT("direction");
	pMonster_direction->properUtype = 40001;
	pMonster_direction->properFlags = 4;
	pMonster_direction->aliasID = 2;
	KBVariant* pMonster_direction_defval = new KBVariant(KBVector3f());
	pMonster_direction->pDefaultVal = pMonster_direction_defval;
	pMonsterModule->propertys.Add(KBTEXT("direction"), pMonster_direction); 

	pMonsterModule->usePropertyDescrAlias = true;
	pMonsterModule->idpropertys.Add((uint16)pMonster_direction->aliasID, pMonster_direction);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Monster), property(direction / 40001).");

	Property* pMonster_spaceID = new Property();
	pMonster_spaceID->name = KBTEXT("spaceID");
	pMonster_spaceID->properUtype = 40002;
	pMonster_spaceID->properFlags = 16;
	pMonster_spaceID->aliasID = 3;
	KBVariant* pMonster_spaceID_defval = new KBVariant((uint32)safe_atoi64(KBTEXT("")));
	pMonster_spaceID->pDefaultVal = pMonster_spaceID_defval;
	pMonsterModule->propertys.Add(KBTEXT("spaceID"), pMonster_spaceID); 

	pMonsterModule->usePropertyDescrAlias = true;
	pMonsterModule->idpropertys.Add((uint16)pMonster_spaceID->aliasID, pMonster_spaceID);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Monster), property(spaceID / 40002).");

	Property* pMonster_HP = new Property();
	pMonster_HP->name = KBTEXT("HP");
	pMonster_HP->properUtype = 47001;
	pMonster_HP->properFlags = 4;
	pMonster_HP->aliasID = 4;
	KBVariant* pMonster_HP_defval = new KBVariant((int32)safe_atoi64(KBTEXT("0")));
	pMonster_HP->pDefaultVal = pMonster_HP_defval;
	pMonsterModule->propertys.Add(KBTEXT("HP"), pMonster_HP); 

	pMonsterModule->usePropertyDescrAlias = true;
	pMonsterModule->idpropertys.Add((uint16)pMonster_HP->aliasID, pMonster_HP);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Monster), property(HP / 47001).");

	Property* pMonster_HP_Max = new Property();
	pMonster_HP_Max->name = KBTEXT("HP_Max");
	pMonster_HP_Max->properUtype = 47002;
	pMonster_HP_Max->properFlags = 4;
	pMonster_HP_Max->aliasID = 5;
	KBVariant* pMonster_HP_Max_defval = new KBVariant((int32)safe_atoi64(KBTEXT("0")));
	pMonster_HP_Max->pDefaultVal = pMonster_HP_Max_defval;
	pMonsterModule->propertys.Add(KBTEXT("HP_Max"), pMonster_HP_Max); 

	pMonsterModule->usePropertyDescrAlias = true;
	pMonsterModule->idpropertys.Add((uint16)pMonster_HP_Max->aliasID, pMonster_HP_Max);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Monster), property(HP_Max / 47002).");

	Property* pMonster_MP = new Property();
	pMonster_MP->name = KBTEXT("MP");
	pMonster_MP->properUtype = 47003;
	pMonster_MP->properFlags = 4;
	pMonster_MP->aliasID = 6;
	KBVariant* pMonster_MP_defval = new KBVariant((int32)safe_atoi64(KBTEXT("0")));
	pMonster_MP->pDefaultVal = pMonster_MP_defval;
	pMonsterModule->propertys.Add(KBTEXT("MP"), pMonster_MP); 

	pMonsterModule->usePropertyDescrAlias = true;
	pMonsterModule->idpropertys.Add((uint16)pMonster_MP->aliasID, pMonster_MP);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Monster), property(MP / 47003).");

	Property* pMonster_MP_Max = new Property();
	pMonster_MP_Max->name = KBTEXT("MP_Max");
	pMonster_MP_Max->properUtype = 47004;
	pMonster_MP_Max->properFlags = 4;
	pMonster_MP_Max->aliasID = 7;
	KBVariant* pMonster_MP_Max_defval = new KBVariant((int32)safe_atoi64(KBTEXT("0")));
	pMonster_MP_Max->pDefaultVal = pMonster_MP_Max_defval;
	pMonsterModule->propertys.Add(KBTEXT("MP_Max"), pMonster_MP_Max); 

	pMonsterModule->usePropertyDescrAlias = true;
	pMonsterModule->idpropertys.Add((uint16)pMonster_MP_Max->aliasID, pMonster_MP_Max);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Monster), property(MP_Max / 47004).");

	Property* pMonster_entityNO = new Property();
	pMonster_entityNO->name = KBTEXT("entityNO");
	pMonster_entityNO->properUtype = 51007;
	pMonster_entityNO->properFlags = 4;
	pMonster_entityNO->aliasID = 8;
	KBVariant* pMonster_entityNO_defval = new KBVariant((uint32)safe_atoi64(KBTEXT("0")));
	pMonster_entityNO->pDefaultVal = pMonster_entityNO_defval;
	pMonsterModule->propertys.Add(KBTEXT("entityNO"), pMonster_entityNO); 

	pMonsterModule->usePropertyDescrAlias = true;
	pMonsterModule->idpropertys.Add((uint16)pMonster_entityNO->aliasID, pMonster_entityNO);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Monster), property(entityNO / 51007).");

	Property* pMonster_forbids = new Property();
	pMonster_forbids->name = KBTEXT("forbids");
	pMonster_forbids->properUtype = 47005;
	pMonster_forbids->properFlags = 4;
	pMonster_forbids->aliasID = 9;
	KBVariant* pMonster_forbids_defval = new KBVariant((int32)safe_atoi64(KBTEXT("0")));
	pMonster_forbids->pDefaultVal = pMonster_forbids_defval;
	pMonsterModule->propertys.Add(KBTEXT("forbids"), pMonster_forbids); 

	pMonsterModule->usePropertyDescrAlias = true;
	pMonsterModule->idpropertys.Add((uint16)pMonster_forbids->aliasID, pMonster_forbids);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Monster), property(forbids / 47005).");

	Property* pMonster_modelID = new Property();
	pMonster_modelID->name = KBTEXT("modelID");
	pMonster_modelID->properUtype = 41006;
	pMonster_modelID->properFlags = 4;
	pMonster_modelID->aliasID = 10;
	KBVariant* pMonster_modelID_defval = new KBVariant((uint32)safe_atoi64(KBTEXT("0")));
	pMonster_modelID->pDefaultVal = pMonster_modelID_defval;
	pMonsterModule->propertys.Add(KBTEXT("modelID"), pMonster_modelID); 

	pMonsterModule->usePropertyDescrAlias = true;
	pMonsterModule->idpropertys.Add((uint16)pMonster_modelID->aliasID, pMonster_modelID);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Monster), property(modelID / 41006).");

	Property* pMonster_modelScale = new Property();
	pMonster_modelScale->name = KBTEXT("modelScale");
	pMonster_modelScale->properUtype = 41007;
	pMonster_modelScale->properFlags = 4;
	pMonster_modelScale->aliasID = 11;
	KBVariant* pMonster_modelScale_defval = new KBVariant((uint8)safe_atoi64(KBTEXT("30")));
	pMonster_modelScale->pDefaultVal = pMonster_modelScale_defval;
	pMonsterModule->propertys.Add(KBTEXT("modelScale"), pMonster_modelScale); 

	pMonsterModule->usePropertyDescrAlias = true;
	pMonsterModule->idpropertys.Add((uint16)pMonster_modelScale->aliasID, pMonster_modelScale);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Monster), property(modelScale / 41007).");

	Property* pMonster_moveSpeed = new Property();
	pMonster_moveSpeed->name = KBTEXT("moveSpeed");
	pMonster_moveSpeed->properUtype = 32;
	pMonster_moveSpeed->properFlags = 4;
	pMonster_moveSpeed->aliasID = 12;
	KBVariant* pMonster_moveSpeed_defval = new KBVariant((uint8)safe_atoi64(KBTEXT("50")));
	pMonster_moveSpeed->pDefaultVal = pMonster_moveSpeed_defval;
	pMonsterModule->propertys.Add(KBTEXT("moveSpeed"), pMonster_moveSpeed); 

	pMonsterModule->usePropertyDescrAlias = true;
	pMonsterModule->idpropertys.Add((uint16)pMonster_moveSpeed->aliasID, pMonster_moveSpeed);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Monster), property(moveSpeed / 32).");

	Property* pMonster_name = new Property();
	pMonster_name->name = KBTEXT("name");
	pMonster_name->properUtype = 41003;
	pMonster_name->properFlags = 4;
	pMonster_name->aliasID = 13;
	KBVariant* pMonster_name_defval = new KBVariant(KBString());
	pMonster_name->pDefaultVal = pMonster_name_defval;
	pMonsterModule->propertys.Add(KBTEXT("name"), pMonster_name); 

	pMonsterModule->usePropertyDescrAlias = true;
	pMonsterModule->idpropertys.Add((uint16)pMonster_name->aliasID, pMonster_name);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Monster), property(name / 41003).");

	Property* pMonster_state = new Property();
	pMonster_state->name = KBTEXT("state");
	pMonster_state->properUtype = 47006;
	pMonster_state->properFlags = 4;
	pMonster_state->aliasID = 14;
	KBVariant* pMonster_state_defval = new KBVariant((int8)safe_atoi64(KBTEXT("0")));
	pMonster_state->pDefaultVal = pMonster_state_defval;
	pMonsterModule->propertys.Add(KBTEXT("state"), pMonster_state); 

	pMonsterModule->usePropertyDescrAlias = true;
	pMonsterModule->idpropertys.Add((uint16)pMonster_state->aliasID, pMonster_state);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Monster), property(state / 47006).");

	Property* pMonster_subState = new Property();
	pMonster_subState->name = KBTEXT("subState");
	pMonster_subState->properUtype = 47007;
	pMonster_subState->properFlags = 4;
	pMonster_subState->aliasID = 15;
	KBVariant* pMonster_subState_defval = new KBVariant((uint8)safe_atoi64(KBTEXT("")));
	pMonster_subState->pDefaultVal = pMonster_subState_defval;
	pMonsterModule->propertys.Add(KBTEXT("subState"), pMonster_subState); 

	pMonsterModule->usePropertyDescrAlias = true;
	pMonsterModule->idpropertys.Add((uint16)pMonster_subState->aliasID, pMonster_subState);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Monster), property(subState / 47007).");

	Property* pMonster_uid = new Property();
	pMonster_uid->name = KBTEXT("uid");
	pMonster_uid->properUtype = 41004;
	pMonster_uid->properFlags = 4;
	pMonster_uid->aliasID = 16;
	KBVariant* pMonster_uid_defval = new KBVariant((uint32)safe_atoi64(KBTEXT("0")));
	pMonster_uid->pDefaultVal = pMonster_uid_defval;
	pMonsterModule->propertys.Add(KBTEXT("uid"), pMonster_uid); 

	pMonsterModule->usePropertyDescrAlias = true;
	pMonsterModule->idpropertys.Add((uint16)pMonster_uid->aliasID, pMonster_uid);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Monster), property(uid / 41004).");

	Property* pMonster_utype = new Property();
	pMonster_utype->name = KBTEXT("utype");
	pMonster_utype->properUtype = 41005;
	pMonster_utype->properFlags = 4;
	pMonster_utype->aliasID = 17;
	KBVariant* pMonster_utype_defval = new KBVariant((uint32)safe_atoi64(KBTEXT("0")));
	pMonster_utype->pDefaultVal = pMonster_utype_defval;
	pMonsterModule->propertys.Add(KBTEXT("utype"), pMonster_utype); 

	pMonsterModule->usePropertyDescrAlias = true;
	pMonsterModule->idpropertys.Add((uint16)pMonster_utype->aliasID, pMonster_utype);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Monster), property(utype / 41005).");

	KBArray<DATATYPE_BASE*> Monster_recvDamage_args;
	Monster_recvDamage_args.Add(EntityDef::id2datatypes[8]);
	Monster_recvDamage_args.Add(EntityDef::id2datatypes[8]);
	Monster_recvDamage_args.Add(EntityDef::id2datatypes[8]);
	Monster_recvDamage_args.Add(EntityDef::id2datatypes[8]);

	Method* pMonster_recvDamage = new Method();
	pMonster_recvDamage->name = KBTEXT("recvDamage");
	pMonster_recvDamage->methodUtype = 34;
	pMonster_recvDamage->aliasID = 1;
	pMonster_recvDamage->args = Monster_recvDamage_args;

	pMonsterModule->methods.Add(KBTEXT("recvDamage"), pMonster_recvDamage); 
	pMonsterModule->useMethodDescrAlias = true;
	pMonsterModule->idmethods.Add((uint16)pMonster_recvDamage->aliasID, pMonster_recvDamage);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Monster), method(recvDamage / 34).");

	ScriptModule* pNPCModule = new ScriptModule("NPC", 6);
	EntityDef::moduledefs.Add(KBTEXT("NPC"), pNPCModule);
	EntityDef::idmoduledefs.Add(6, pNPCModule);

	Property* pNPC_position = new Property();
	pNPC_position->name = KBTEXT("position");
	pNPC_position->properUtype = 40000;
	pNPC_position->properFlags = 4;
	pNPC_position->aliasID = 1;
	KBVariant* pNPC_position_defval = new KBVariant(KBVector3f());
	pNPC_position->pDefaultVal = pNPC_position_defval;
	pNPCModule->propertys.Add(KBTEXT("position"), pNPC_position); 

	pNPCModule->usePropertyDescrAlias = true;
	pNPCModule->idpropertys.Add((uint16)pNPC_position->aliasID, pNPC_position);

	//DEBUG_MSG("EntityDef::initScriptModules: add(NPC), property(position / 40000).");

	Property* pNPC_direction = new Property();
	pNPC_direction->name = KBTEXT("direction");
	pNPC_direction->properUtype = 40001;
	pNPC_direction->properFlags = 4;
	pNPC_direction->aliasID = 2;
	KBVariant* pNPC_direction_defval = new KBVariant(KBVector3f());
	pNPC_direction->pDefaultVal = pNPC_direction_defval;
	pNPCModule->propertys.Add(KBTEXT("direction"), pNPC_direction); 

	pNPCModule->usePropertyDescrAlias = true;
	pNPCModule->idpropertys.Add((uint16)pNPC_direction->aliasID, pNPC_direction);

	//DEBUG_MSG("EntityDef::initScriptModules: add(NPC), property(direction / 40001).");

	Property* pNPC_spaceID = new Property();
	pNPC_spaceID->name = KBTEXT("spaceID");
	pNPC_spaceID->properUtype = 40002;
	pNPC_spaceID->properFlags = 16;
	pNPC_spaceID->aliasID = 3;
	KBVariant* pNPC_spaceID_defval = new KBVariant((uint32)safe_atoi64(KBTEXT("")));
	pNPC_spaceID->pDefaultVal = pNPC_spaceID_defval;
	pNPCModule->propertys.Add(KBTEXT("spaceID"), pNPC_spaceID); 

	pNPCModule->usePropertyDescrAlias = true;
	pNPCModule->idpropertys.Add((uint16)pNPC_spaceID->aliasID, pNPC_spaceID);

	//DEBUG_MSG("EntityDef::initScriptModules: add(NPC), property(spaceID / 40002).");

	Property* pNPC_entityNO = new Property();
	pNPC_entityNO->name = KBTEXT("entityNO");
	pNPC_entityNO->properUtype = 51007;
	pNPC_entityNO->properFlags = 4;
	pNPC_entityNO->aliasID = 4;
	KBVariant* pNPC_entityNO_defval = new KBVariant((uint32)safe_atoi64(KBTEXT("0")));
	pNPC_entityNO->pDefaultVal = pNPC_entityNO_defval;
	pNPCModule->propertys.Add(KBTEXT("entityNO"), pNPC_entityNO); 

	pNPCModule->usePropertyDescrAlias = true;
	pNPCModule->idpropertys.Add((uint16)pNPC_entityNO->aliasID, pNPC_entityNO);

	//DEBUG_MSG("EntityDef::initScriptModules: add(NPC), property(entityNO / 51007).");

	Property* pNPC_modelID = new Property();
	pNPC_modelID->name = KBTEXT("modelID");
	pNPC_modelID->properUtype = 41006;
	pNPC_modelID->properFlags = 4;
	pNPC_modelID->aliasID = 5;
	KBVariant* pNPC_modelID_defval = new KBVariant((uint32)safe_atoi64(KBTEXT("0")));
	pNPC_modelID->pDefaultVal = pNPC_modelID_defval;
	pNPCModule->propertys.Add(KBTEXT("modelID"), pNPC_modelID); 

	pNPCModule->usePropertyDescrAlias = true;
	pNPCModule->idpropertys.Add((uint16)pNPC_modelID->aliasID, pNPC_modelID);

	//DEBUG_MSG("EntityDef::initScriptModules: add(NPC), property(modelID / 41006).");

	Property* pNPC_modelScale = new Property();
	pNPC_modelScale->name = KBTEXT("modelScale");
	pNPC_modelScale->properUtype = 41007;
	pNPC_modelScale->properFlags = 4;
	pNPC_modelScale->aliasID = 6;
	KBVariant* pNPC_modelScale_defval = new KBVariant((uint8)safe_atoi64(KBTEXT("30")));
	pNPC_modelScale->pDefaultVal = pNPC_modelScale_defval;
	pNPCModule->propertys.Add(KBTEXT("modelScale"), pNPC_modelScale); 

	pNPCModule->usePropertyDescrAlias = true;
	pNPCModule->idpropertys.Add((uint16)pNPC_modelScale->aliasID, pNPC_modelScale);

	//DEBUG_MSG("EntityDef::initScriptModules: add(NPC), property(modelScale / 41007).");

	Property* pNPC_moveSpeed = new Property();
	pNPC_moveSpeed->name = KBTEXT("moveSpeed");
	pNPC_moveSpeed->properUtype = 43;
	pNPC_moveSpeed->properFlags = 4;
	pNPC_moveSpeed->aliasID = 7;
	KBVariant* pNPC_moveSpeed_defval = new KBVariant((uint8)safe_atoi64(KBTEXT("50")));
	pNPC_moveSpeed->pDefaultVal = pNPC_moveSpeed_defval;
	pNPCModule->propertys.Add(KBTEXT("moveSpeed"), pNPC_moveSpeed); 

	pNPCModule->usePropertyDescrAlias = true;
	pNPCModule->idpropertys.Add((uint16)pNPC_moveSpeed->aliasID, pNPC_moveSpeed);

	//DEBUG_MSG("EntityDef::initScriptModules: add(NPC), property(moveSpeed / 43).");

	Property* pNPC_name = new Property();
	pNPC_name->name = KBTEXT("name");
	pNPC_name->properUtype = 41003;
	pNPC_name->properFlags = 4;
	pNPC_name->aliasID = 8;
	KBVariant* pNPC_name_defval = new KBVariant(KBString());
	pNPC_name->pDefaultVal = pNPC_name_defval;
	pNPCModule->propertys.Add(KBTEXT("name"), pNPC_name); 

	pNPCModule->usePropertyDescrAlias = true;
	pNPCModule->idpropertys.Add((uint16)pNPC_name->aliasID, pNPC_name);

	//DEBUG_MSG("EntityDef::initScriptModules: add(NPC), property(name / 41003).");

	Property* pNPC_uid = new Property();
	pNPC_uid->name = KBTEXT("uid");
	pNPC_uid->properUtype = 41004;
	pNPC_uid->properFlags = 4;
	pNPC_uid->aliasID = 9;
	KBVariant* pNPC_uid_defval = new KBVariant((uint32)safe_atoi64(KBTEXT("0")));
	pNPC_uid->pDefaultVal = pNPC_uid_defval;
	pNPCModule->propertys.Add(KBTEXT("uid"), pNPC_uid); 

	pNPCModule->usePropertyDescrAlias = true;
	pNPCModule->idpropertys.Add((uint16)pNPC_uid->aliasID, pNPC_uid);

	//DEBUG_MSG("EntityDef::initScriptModules: add(NPC), property(uid / 41004).");

	Property* pNPC_utype = new Property();
	pNPC_utype->name = KBTEXT("utype");
	pNPC_utype->properUtype = 41005;
	pNPC_utype->properFlags = 4;
	pNPC_utype->aliasID = 10;
	KBVariant* pNPC_utype_defval = new KBVariant((uint32)safe_atoi64(KBTEXT("0")));
	pNPC_utype->pDefaultVal = pNPC_utype_defval;
	pNPCModule->propertys.Add(KBTEXT("utype"), pNPC_utype); 

	pNPCModule->usePropertyDescrAlias = true;
	pNPCModule->idpropertys.Add((uint16)pNPC_utype->aliasID, pNPC_utype);

	//DEBUG_MSG("EntityDef::initScriptModules: add(NPC), property(utype / 41005).");

	pNPCModule->useMethodDescrAlias = true;
	ScriptModule* pGateModule = new ScriptModule("Gate", 7);
	EntityDef::moduledefs.Add(KBTEXT("Gate"), pGateModule);
	EntityDef::idmoduledefs.Add(7, pGateModule);

	Property* pGate_position = new Property();
	pGate_position->name = KBTEXT("position");
	pGate_position->properUtype = 40000;
	pGate_position->properFlags = 4;
	pGate_position->aliasID = 1;
	KBVariant* pGate_position_defval = new KBVariant(KBVector3f());
	pGate_position->pDefaultVal = pGate_position_defval;
	pGateModule->propertys.Add(KBTEXT("position"), pGate_position); 

	pGateModule->usePropertyDescrAlias = true;
	pGateModule->idpropertys.Add((uint16)pGate_position->aliasID, pGate_position);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Gate), property(position / 40000).");

	Property* pGate_direction = new Property();
	pGate_direction->name = KBTEXT("direction");
	pGate_direction->properUtype = 40001;
	pGate_direction->properFlags = 4;
	pGate_direction->aliasID = 2;
	KBVariant* pGate_direction_defval = new KBVariant(KBVector3f());
	pGate_direction->pDefaultVal = pGate_direction_defval;
	pGateModule->propertys.Add(KBTEXT("direction"), pGate_direction); 

	pGateModule->usePropertyDescrAlias = true;
	pGateModule->idpropertys.Add((uint16)pGate_direction->aliasID, pGate_direction);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Gate), property(direction / 40001).");

	Property* pGate_spaceID = new Property();
	pGate_spaceID->name = KBTEXT("spaceID");
	pGate_spaceID->properUtype = 40002;
	pGate_spaceID->properFlags = 16;
	pGate_spaceID->aliasID = 3;
	KBVariant* pGate_spaceID_defval = new KBVariant((uint32)safe_atoi64(KBTEXT("")));
	pGate_spaceID->pDefaultVal = pGate_spaceID_defval;
	pGateModule->propertys.Add(KBTEXT("spaceID"), pGate_spaceID); 

	pGateModule->usePropertyDescrAlias = true;
	pGateModule->idpropertys.Add((uint16)pGate_spaceID->aliasID, pGate_spaceID);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Gate), property(spaceID / 40002).");

	Property* pGate_entityNO = new Property();
	pGate_entityNO->name = KBTEXT("entityNO");
	pGate_entityNO->properUtype = 51007;
	pGate_entityNO->properFlags = 4;
	pGate_entityNO->aliasID = 4;
	KBVariant* pGate_entityNO_defval = new KBVariant((uint32)safe_atoi64(KBTEXT("0")));
	pGate_entityNO->pDefaultVal = pGate_entityNO_defval;
	pGateModule->propertys.Add(KBTEXT("entityNO"), pGate_entityNO); 

	pGateModule->usePropertyDescrAlias = true;
	pGateModule->idpropertys.Add((uint16)pGate_entityNO->aliasID, pGate_entityNO);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Gate), property(entityNO / 51007).");

	Property* pGate_modelID = new Property();
	pGate_modelID->name = KBTEXT("modelID");
	pGate_modelID->properUtype = 41006;
	pGate_modelID->properFlags = 4;
	pGate_modelID->aliasID = 5;
	KBVariant* pGate_modelID_defval = new KBVariant((uint32)safe_atoi64(KBTEXT("0")));
	pGate_modelID->pDefaultVal = pGate_modelID_defval;
	pGateModule->propertys.Add(KBTEXT("modelID"), pGate_modelID); 

	pGateModule->usePropertyDescrAlias = true;
	pGateModule->idpropertys.Add((uint16)pGate_modelID->aliasID, pGate_modelID);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Gate), property(modelID / 41006).");

	Property* pGate_modelScale = new Property();
	pGate_modelScale->name = KBTEXT("modelScale");
	pGate_modelScale->properUtype = 41007;
	pGate_modelScale->properFlags = 4;
	pGate_modelScale->aliasID = 6;
	KBVariant* pGate_modelScale_defval = new KBVariant((uint8)safe_atoi64(KBTEXT("30")));
	pGate_modelScale->pDefaultVal = pGate_modelScale_defval;
	pGateModule->propertys.Add(KBTEXT("modelScale"), pGate_modelScale); 

	pGateModule->usePropertyDescrAlias = true;
	pGateModule->idpropertys.Add((uint16)pGate_modelScale->aliasID, pGate_modelScale);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Gate), property(modelScale / 41007).");

	Property* pGate_name = new Property();
	pGate_name->name = KBTEXT("name");
	pGate_name->properUtype = 41003;
	pGate_name->properFlags = 4;
	pGate_name->aliasID = 7;
	KBVariant* pGate_name_defval = new KBVariant(KBString());
	pGate_name->pDefaultVal = pGate_name_defval;
	pGateModule->propertys.Add(KBTEXT("name"), pGate_name); 

	pGateModule->usePropertyDescrAlias = true;
	pGateModule->idpropertys.Add((uint16)pGate_name->aliasID, pGate_name);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Gate), property(name / 41003).");

	Property* pGate_uid = new Property();
	pGate_uid->name = KBTEXT("uid");
	pGate_uid->properUtype = 41004;
	pGate_uid->properFlags = 4;
	pGate_uid->aliasID = 8;
	KBVariant* pGate_uid_defval = new KBVariant((uint32)safe_atoi64(KBTEXT("0")));
	pGate_uid->pDefaultVal = pGate_uid_defval;
	pGateModule->propertys.Add(KBTEXT("uid"), pGate_uid); 

	pGateModule->usePropertyDescrAlias = true;
	pGateModule->idpropertys.Add((uint16)pGate_uid->aliasID, pGate_uid);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Gate), property(uid / 41004).");

	Property* pGate_utype = new Property();
	pGate_utype->name = KBTEXT("utype");
	pGate_utype->properUtype = 41005;
	pGate_utype->properFlags = 4;
	pGate_utype->aliasID = 9;
	KBVariant* pGate_utype_defval = new KBVariant((uint32)safe_atoi64(KBTEXT("0")));
	pGate_utype->pDefaultVal = pGate_utype_defval;
	pGateModule->propertys.Add(KBTEXT("utype"), pGate_utype); 

	pGateModule->usePropertyDescrAlias = true;
	pGateModule->idpropertys.Add((uint16)pGate_utype->aliasID, pGate_utype);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Gate), property(utype / 41005).");

	pGateModule->useMethodDescrAlias = true;
}

void EntityDef::initDefTypes()
{
	{
		uint16 utype = 2;
		KBString typeName = KBTEXT("ENTITY_SUBSTATE");
		KBString name = KBTEXT("UINT8");
		DATATYPE_BASE** fPtr = EntityDef::datatypes.Find(name);
		DATATYPE_BASE* pVal = fPtr != NULL ? *fPtr : NULL;
		EntityDef::datatypes.Add(typeName, pVal);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 3;
		KBString typeName = KBTEXT("UINT16");
		KBString name = KBTEXT("UINT16");
		DATATYPE_BASE** fPtr = EntityDef::datatypes.Find(name);
		DATATYPE_BASE* pVal = fPtr != NULL ? *fPtr : NULL;
		EntityDef::datatypes.Add(typeName, pVal);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 5;
		KBString typeName = KBTEXT("UID");
		KBString name = KBTEXT("UINT64");
		DATATYPE_BASE** fPtr = EntityDef::datatypes.Find(name);
		DATATYPE_BASE* pVal = fPtr != NULL ? *fPtr : NULL;
		EntityDef::datatypes.Add(typeName, pVal);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 4;
		KBString typeName = KBTEXT("ENTITY_UTYPE");
		KBString name = KBTEXT("UINT32");
		DATATYPE_BASE** fPtr = EntityDef::datatypes.Find(name);
		DATATYPE_BASE* pVal = fPtr != NULL ? *fPtr : NULL;
		EntityDef::datatypes.Add(typeName, pVal);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 6;
		KBString typeName = KBTEXT("ENTITY_STATE");
		KBString name = KBTEXT("INT8");
		DATATYPE_BASE** fPtr = EntityDef::datatypes.Find(name);
		DATATYPE_BASE* pVal = fPtr != NULL ? *fPtr : NULL;
		EntityDef::datatypes.Add(typeName, pVal);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 7;
		KBString typeName = KBTEXT("INT16");
		KBString name = KBTEXT("INT16");
		DATATYPE_BASE** fPtr = EntityDef::datatypes.Find(name);
		DATATYPE_BASE* pVal = fPtr != NULL ? *fPtr : NULL;
		EntityDef::datatypes.Add(typeName, pVal);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 8;
		KBString typeName = KBTEXT("ENTITY_FORBIDS");
		KBString name = KBTEXT("INT32");
		DATATYPE_BASE** fPtr = EntityDef::datatypes.Find(name);
		DATATYPE_BASE* pVal = fPtr != NULL ? *fPtr : NULL;
		EntityDef::datatypes.Add(typeName, pVal);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 9;
		KBString typeName = KBTEXT("INT64");
		KBString name = KBTEXT("INT64");
		DATATYPE_BASE** fPtr = EntityDef::datatypes.Find(name);
		DATATYPE_BASE* pVal = fPtr != NULL ? *fPtr : NULL;
		EntityDef::datatypes.Add(typeName, pVal);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 1;
		KBString typeName = KBTEXT("STRING");
		KBString name = KBTEXT("STRING");
		DATATYPE_BASE** fPtr = EntityDef::datatypes.Find(name);
		DATATYPE_BASE* pVal = fPtr != NULL ? *fPtr : NULL;
		EntityDef::datatypes.Add(typeName, pVal);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 12;
		KBString typeName = KBTEXT("UNICODE");
		KBString name = KBTEXT("UNICODE");
		DATATYPE_BASE** fPtr = EntityDef::datatypes.Find(name);
		DATATYPE_BASE* pVal = fPtr != NULL ? *fPtr : NULL;
		EntityDef::datatypes.Add(typeName, pVal);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 13;
		KBString typeName = KBTEXT("FLOAT");
		KBString name = KBTEXT("FLOAT");
		DATATYPE_BASE** fPtr = EntityDef::datatypes.Find(name);
		DATATYPE_BASE* pVal = fPtr != NULL ? *fPtr : NULL;
		EntityDef::datatypes.Add(typeName, pVal);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 14;
		KBString typeName = KBTEXT("DOUBLE");
		KBString name = KBTEXT("DOUBLE");
		DATATYPE_BASE** fPtr = EntityDef::datatypes.Find(name);
		DATATYPE_BASE* pVal = fPtr != NULL ? *fPtr : NULL;
		EntityDef::datatypes.Add(typeName, pVal);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 10;
		KBString typeName = KBTEXT("UID1");
		KBString name = KBTEXT("PYTHON");
		DATATYPE_BASE** fPtr = EntityDef::datatypes.Find(name);
		DATATYPE_BASE* pVal = fPtr != NULL ? *fPtr : NULL;
		EntityDef::datatypes.Add(typeName, pVal);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 10;
		KBString typeName = KBTEXT("PY_DICT");
		KBString name = KBTEXT("PY_DICT");
		DATATYPE_BASE** fPtr = EntityDef::datatypes.Find(name);
		DATATYPE_BASE* pVal = fPtr != NULL ? *fPtr : NULL;
		EntityDef::datatypes.Add(typeName, pVal);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 10;
		KBString typeName = KBTEXT("PY_TUPLE");
		KBString name = KBTEXT("PY_TUPLE");
		DATATYPE_BASE** fPtr = EntityDef::datatypes.Find(name);
		DATATYPE_BASE* pVal = fPtr != NULL ? *fPtr : NULL;
		EntityDef::datatypes.Add(typeName, pVal);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 10;
		KBString typeName = KBTEXT("PY_LIST");
		KBString name = KBTEXT("PY_LIST");
		DATATYPE_BASE** fPtr = EntityDef::datatypes.Find(name);
		DATATYPE_BASE* pVal = fPtr != NULL ? *fPtr : NULL;
		EntityDef::datatypes.Add(typeName, pVal);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 20;
		KBString typeName = KBTEXT("ENTITYCALL");
		KBString name = KBTEXT("ENTITYCALL");
		DATATYPE_BASE** fPtr = EntityDef::datatypes.Find(name);
		DATATYPE_BASE* pVal = fPtr != NULL ? *fPtr : NULL;
		EntityDef::datatypes.Add(typeName, pVal);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 11;
		KBString typeName = KBTEXT("BLOB");
		KBString name = KBTEXT("BLOB");
		DATATYPE_BASE** fPtr = EntityDef::datatypes.Find(name);
		DATATYPE_BASE* pVal = fPtr != NULL ? *fPtr : NULL;
		EntityDef::datatypes.Add(typeName, pVal);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 15;
		KBString typeName = KBTEXT("VECTOR2");
		KBString name = KBTEXT("VECTOR2");
		DATATYPE_BASE** fPtr = EntityDef::datatypes.Find(name);
		DATATYPE_BASE* pVal = fPtr != NULL ? *fPtr : NULL;
		EntityDef::datatypes.Add(typeName, pVal);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 16;
		KBString typeName = KBTEXT("DIRECTION3D");
		KBString name = KBTEXT("VECTOR3");
		DATATYPE_BASE** fPtr = EntityDef::datatypes.Find(name);
		DATATYPE_BASE* pVal = fPtr != NULL ? *fPtr : NULL;
		EntityDef::datatypes.Add(typeName, pVal);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 17;
		KBString typeName = KBTEXT("VECTOR4");
		KBString name = KBTEXT("VECTOR4");
		DATATYPE_BASE** fPtr = EntityDef::datatypes.Find(name);
		DATATYPE_BASE* pVal = fPtr != NULL ? *fPtr : NULL;
		EntityDef::datatypes.Add(typeName, pVal);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 22;
		KBString typeName = KBTEXT("ENTITY_FORBID_COUNTER");
		DATATYPE_ENTITY_FORBID_COUNTER* pDatatype = new DATATYPE_ENTITY_FORBID_COUNTER();
		EntityDef::datatypes.Add(typeName, (DATATYPE_BASE*)pDatatype);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 23;
		KBString typeName = KBTEXT("ENTITYID_LIST");
		DATATYPE_ENTITYID_LIST* pDatatype = new DATATYPE_ENTITYID_LIST();
		EntityDef::datatypes.Add(typeName, (DATATYPE_BASE*)pDatatype);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 24;
		KBString typeName = KBTEXT("AVATAR_DATA");
		DATATYPE_AVATAR_DATA* pDatatype = new DATATYPE_AVATAR_DATA();
		EntityDef::datatypes.Add(typeName, (DATATYPE_BASE*)pDatatype);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 25;
		KBString typeName = KBTEXT("AVATAR_INFOS");
		DATATYPE_AVATAR_INFOS* pDatatype = new DATATYPE_AVATAR_INFOS();
		EntityDef::datatypes.Add(typeName, (DATATYPE_BASE*)pDatatype);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 26;
		KBString typeName = KBTEXT("AVATAR_INFOS_LIST");
		DATATYPE_AVATAR_INFOS_LIST* pDatatype = new DATATYPE_AVATAR_INFOS_LIST();
		EntityDef::datatypes.Add(typeName, (DATATYPE_BASE*)pDatatype);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 28;
		KBString typeName = KBTEXT("BAG");
		DATATYPE_BAG* pDatatype = new DATATYPE_BAG();
		EntityDef::datatypes.Add(typeName, (DATATYPE_BASE*)pDatatype);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 31;
		KBString typeName = KBTEXT("EXAMPLES");
		DATATYPE_EXAMPLES* pDatatype = new DATATYPE_EXAMPLES();
		EntityDef::datatypes.Add(typeName, (DATATYPE_BASE*)pDatatype);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 32;
		KBString typeName = KBTEXT("AnonymousArray_32");
		DATATYPE_AnonymousArray_32* pDatatype = new DATATYPE_AnonymousArray_32();
		EntityDef::datatypes.Add(typeName, (DATATYPE_BASE*)pDatatype);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 33;
		KBString typeName = KBTEXT("AnonymousArray_33");
		KBString name = KBTEXT("ENTITY_COMPONENT");
		DATATYPE_BASE** fPtr = EntityDef::datatypes.Find(name);
		DATATYPE_BASE* pVal = fPtr != NULL ? *fPtr : NULL;
		EntityDef::datatypes.Add(typeName, pVal);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 34;
		KBString typeName = KBTEXT("AnonymousArray_34");
		KBString name = KBTEXT("ENTITY_COMPONENT");
		DATATYPE_BASE** fPtr = EntityDef::datatypes.Find(name);
		DATATYPE_BASE* pVal = fPtr != NULL ? *fPtr : NULL;
		EntityDef::datatypes.Add(typeName, pVal);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 35;
		KBString typeName = KBTEXT("AnonymousArray_35");
		KBString name = KBTEXT("ENTITY_COMPONENT");
		DATATYPE_BASE** fPtr = EntityDef::datatypes.Find(name);
		DATATYPE_BASE* pVal = fPtr != NULL ? *fPtr : NULL;
		EntityDef::datatypes.Add(typeName, pVal);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	for(auto& Elem : EntityDef::datatypes)
	{
		if(Elem.second)
		{
			Elem.second->bind();
		}
	}
}


}