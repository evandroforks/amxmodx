#include "engine.h"

Message Msg;
CVector<int> msgHooks[256];
int msgBlocks[256] = {0};
int msgDest;
int msgType;
float *msgOrigin;
edict_t *msgpEntity;
bool inhook = false;
bool inblock = false;

Message::Message()
{
	msgparam *p = new msgparam;
	m_Params.push_back(p);
	m_CurParam = 0;
}

Message::~Message()
{
	for (size_t i=0; i<m_Params.size(); i++)
		delete m_Params[i];
	
	m_Params.clear();
}

msgparam *Message::AdvPtr()
{
	msgparam *pParam = NULL;

	if (++m_CurParam >= m_Params.size())
	{
		pParam = new msgparam;
		m_Params.push_back(pParam);
	} else {
		pParam = m_Params[m_CurParam];
	}

	return pParam;
}

void Message::AddParam(const char *data, msgtype type)
{
	msgparam *pParam = AdvPtr();

	pParam->szData.assign(data);
	pParam->type = type;
}

void Message::AddParam(int data, msgtype type)
{
	msgparam *pParam = AdvPtr();
	
	pParam->v.iData = data;
	pParam->type = type;
}

void Message::AddParam(float data, msgtype type)
{
	msgparam *pParam = AdvPtr();

	pParam->v.fData = data;
	pParam->type = type;
}

msgtype Message::GetParamType(size_t index)
{
	if (index < 1 || index > m_CurParam)
		return static_cast<msgtype>(0);

	return m_Params[index]->type;
}

float Message::GetParamFloat(size_t index)
{
	if (index < 1 || index > m_CurParam)
		return 0;

	return m_Params[index]->v.fData;
}

const char *Message::GetParamString(size_t index)
{
	if (index < 1 || index > m_CurParam)
		return 0;

	return m_Params[index]->szData.c_str();
}

int Message::GetParamInt(size_t index)
{
	if (index < 1 || index > m_CurParam)
		return 0;

	return m_Params[index]->v.iData;
}

void Message::SetParam(size_t index, float data)
{
	if (index < 1 || index > m_CurParam)
		return;

	m_Params[index]->v.fData = data;
}

void Message::SetParam(size_t index, int data)
{
	if (index < 1 || index > m_CurParam)
		return;

	m_Params[index]->v.iData = data;
}

void Message::SetParam(size_t index, const char *data)
{
	if (index < 1 || index > m_CurParam)
		return;

	m_Params[index]->szData.assign(data);
}

void Message::Reset()
{
	m_CurParam = 0;
}

size_t Message::Params()
{
	return m_CurParam;
}

void Message::Send()
{
	msgparam *pParam = NULL;

	for (size_t i=1; i<=m_CurParam; i++)
	{
		pParam = m_Params[i];
		switch (pParam->type)
		{
		case arg_byte:
			WRITE_BYTE(pParam->v.iData);
			break;
		case arg_char:
			WRITE_CHAR(pParam->v.iData);
			break;
		case arg_short:
			WRITE_SHORT(pParam->v.iData);
			break;
		case arg_long:
			WRITE_LONG(pParam->v.iData);
			break;
		case arg_angle:
			WRITE_ANGLE(pParam->v.fData);
			break;
		case arg_coord:
			WRITE_COORD(pParam->v.fData);
			break;
		case arg_string:
			WRITE_STRING(pParam->szData.c_str());
			break;
		case arg_entity:
			WRITE_ENTITY(pParam->v.iData);
			break;
		}
	}
}

void MessageBegin(int msg_dest, int msg_type, const float *pOrigin, edict_t *ed)
{
	if (msgBlocks[msg_type]) {
		inblock = true;
		msgType = msg_type;
		RETURN_META(MRES_SUPERCEDE);
	} else if (msgHooks[msg_type].size()) {
		inhook = true;
		msgDest = msg_dest;
		msgType = msg_type;
		msgOrigin = (float *)pOrigin;
		msgpEntity = ed;
		RETURN_META(MRES_SUPERCEDE);
	}

	RETURN_META(MRES_IGNORED);
}

void WriteByte(int iValue)
{
	if (inblock) {
		RETURN_META(MRES_SUPERCEDE);
	} else if (inhook) {
		Msg.AddParam(iValue, arg_byte);
		RETURN_META(MRES_SUPERCEDE);
	}

	RETURN_META(MRES_IGNORED);
}

void WriteChar(int iValue)
{
	if (inblock) {
		RETURN_META(MRES_SUPERCEDE);
	} else if (inhook) {
		Msg.AddParam(iValue, arg_char);
		RETURN_META(MRES_SUPERCEDE);
	}

	RETURN_META(MRES_IGNORED);
}

void WriteShort(int iValue)
{
	if (inblock) {
		RETURN_META(MRES_SUPERCEDE);
	} else if (inhook) {
		Msg.AddParam(iValue, arg_short);
		RETURN_META(MRES_SUPERCEDE);
	}

	RETURN_META(MRES_IGNORED);
}

void WriteLong(int iValue)
{
	if (inblock) {
		RETURN_META(MRES_SUPERCEDE);
	} else if (inhook) {
		Msg.AddParam(iValue, arg_long);
		RETURN_META(MRES_SUPERCEDE);
	}

	RETURN_META(MRES_IGNORED);
}

void WriteAngle(float flValue)
{
	if (inblock) {
		RETURN_META(MRES_SUPERCEDE);
	} else if (inhook) {
		Msg.AddParam(flValue, arg_angle);
		RETURN_META(MRES_SUPERCEDE);
	}

	RETURN_META(MRES_IGNORED);
}

void WriteCoord(float flValue)
{
	if (inblock) {
		RETURN_META(MRES_SUPERCEDE);
	} else if (inhook) {
		Msg.AddParam(flValue, arg_coord);
		RETURN_META(MRES_SUPERCEDE);
	}

	RETURN_META(MRES_IGNORED);
}

void WriteString(const char *sz)
{
	if (inblock) {
		RETURN_META(MRES_SUPERCEDE);
	} else if (inhook) {
		Msg.AddParam(sz, arg_string);
		RETURN_META(MRES_SUPERCEDE);
	}

	RETURN_META(MRES_IGNORED);
}

void WriteEntity(int iValue)
{
	if (inblock) {
		RETURN_META(MRES_SUPERCEDE);
	} else if (inhook) {
		Msg.AddParam(iValue, arg_entity);
		RETURN_META(MRES_SUPERCEDE);
	}

	RETURN_META(MRES_IGNORED);
}

void MessageEnd(void)
{
	int mres = 0, mresB = 0;
	unsigned int i = 0;
	if (inblock) {
		inblock = false;
		if (msgBlocks[msgType] == BLOCK_ONCE)
			msgBlocks[msgType] = BLOCK_NOT;
		RETURN_META(MRES_SUPERCEDE);
	} else if (inhook) {
		for (i=0; i<msgHooks[msgType].size(); i++)
		{
				mresB = MF_ExecuteForward(msgHooks[msgType].at(i), msgType, msgDest, ENTINDEX(msgpEntity));
				if (mresB > mres)
						mres = mresB;
		}
		inhook = false;
		if (mres & 1)
		{
			Msg.Reset();
			RETURN_META(MRES_SUPERCEDE);
		}
		MESSAGE_BEGIN(msgDest, msgType, msgOrigin, msgpEntity);
			Msg.Send();
		MESSAGE_END();
		Msg.Reset();
		RETURN_META(MRES_SUPERCEDE);
	}

	RETURN_META(MRES_IGNORED);
}

static cell AMX_NATIVE_CALL register_message(AMX *amx, cell *params)
{
	int len;
	char *name = MF_GetAmxString(amx, params[2], 0, &len);
	if (params[1]>0 && params[1] < 256) {
		int id = MF_RegisterSPForwardByName(amx, name, FP_CELL, FP_CELL, FP_CELL, FP_CELL, FP_DONE);
		if (id != -1)
		{
			msgHooks[params[1]].push_back(id);
			return id;
		} else {
			MF_LogError(amx, AMX_ERR_NOTFOUND, "Could not find function \"%s\"", name);
			return -1;
		}
	}

	return 0;
}

static cell AMX_NATIVE_CALL set_msg_block(AMX *amx, cell *params)
{
	int msgid = params[1];
	int block = params[2];

	if (msgid < 1 || msgid > 255) {
		MF_RaiseAmxError(amx, AMX_ERR_NATIVE);
		return 0;
	}

	msgBlocks[msgid] = block;

	return 1;
}

static cell AMX_NATIVE_CALL get_msg_block(AMX *amx, cell *params)
{
	int msgid = params[1];

	if (msgid < 1 || msgid > 255) {
		MF_RaiseAmxError(amx, AMX_ERR_NATIVE);
		return 0;
	}

	return msgBlocks[msgid];
}

static cell AMX_NATIVE_CALL get_msg_args(AMX *amx, cell *params)
{
	return Msg.Params();
}

static cell AMX_NATIVE_CALL get_msg_argtype(AMX *amx, cell *params)
{
	size_t argn = static_cast<size_t>(params[1]);

	if (!inhook || argn > Msg.Params())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Invalid message argument %d", argn);
		return 0;
	}

	return Msg.GetParamType(argn);
}

static cell AMX_NATIVE_CALL get_msg_arg_int(AMX *amx, cell *params)
{
	size_t argn = static_cast<size_t>(params[1]);

	if (!inhook || argn > Msg.Params())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Invalid message argument %d", argn);
		return 0;
	}

	return Msg.GetParamInt(argn);
}

static cell AMX_NATIVE_CALL set_msg_arg_int(AMX *amx, cell *params)
{
	size_t argn = static_cast<size_t>(params[1]);

	if (!inhook || argn > Msg.Params())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Invalid message argument %d", argn);
		return 0;
	}

	Msg.SetParam(argn, params[3]);

	return 1;
}

static cell AMX_NATIVE_CALL get_msg_arg_float(AMX *amx, cell *params)
{
	size_t argn = static_cast<size_t>(params[1]);

	if (!inhook || argn > Msg.Params())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Invalid message argument %d", argn);
		return 0;
	}

	return amx_ftoc(Msg.GetParamFloat(argn));
}

static cell AMX_NATIVE_CALL set_msg_arg_float(AMX *amx, cell *params)
{
	size_t argn = static_cast<size_t>(params[1]);

	if (!inhook || argn > Msg.Params())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Invalid message argument %d", argn);
		return 0;
	}

	REAL fVal = amx_ctof(params[3]);

	Msg.SetParam(argn, fVal);

	return 1;
}

static cell AMX_NATIVE_CALL get_msg_arg_string(AMX *amx, cell *params)
{
	size_t argn = static_cast<size_t>(params[1]);

	if (!inhook || argn > Msg.Params())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Invalid message argument %d", argn);
		return 0;
	}

	const char *szVal = Msg.GetParamString(argn);

	return MF_SetAmxString(amx, params[2], szVal, params[3]);
}

static cell AMX_NATIVE_CALL set_msg_arg_string(AMX *amx, cell *params)
{
	size_t argn = static_cast<size_t>(params[1]);
	int iLen;

	if (!inhook || argn > Msg.Params())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Invalid message argument %d", argn);
		return 0;
	}

	char *szVal = MF_GetAmxString(amx, params[2], 0, &iLen);

	Msg.SetParam(argn, szVal);

	return 1;
}

static cell AMX_NATIVE_CALL get_msg_origin(AMX *amx, cell *params)
{
	if (!inhook) {
		MF_RaiseAmxError(amx, AMX_ERR_NATIVE);
		return 0;
	}

	cell *cAddr = MF_GetAmxAddr(amx, params[1]);

	if (msgDest >= MSG_PVS && msgDest <= MSG_PAS_R)
	{
		vec3_t vRet = (Vector)msgOrigin;
		cAddr[0] = amx_ftoc(vRet.x);
		cAddr[1] = amx_ftoc(vRet.y);
		cAddr[2] = amx_ftoc(vRet.z);
	} else {
		cAddr[0] = 0;
		cAddr[1] = 0;
		cAddr[2] = 0;
	}

	return 1;
}

AMX_NATIVE_INFO msg_Natives[] = {
	{"register_message",	register_message},

	{"set_msg_block",		set_msg_block},
	{"get_msg_block",		get_msg_block},

	{"get_msg_args",		get_msg_args},
	{"get_msg_argtype",		get_msg_argtype},
	{"get_msg_arg_int",		get_msg_arg_int},
	{"set_msg_arg_int",		set_msg_arg_int},
	{"get_msg_arg_float",	get_msg_arg_float},
	{"set_msg_arg_float",	set_msg_arg_float},
	{"get_msg_arg_string",	get_msg_arg_string},
	{"set_msg_arg_string",	set_msg_arg_string},
	{"get_msg_origin",		get_msg_origin},

	{NULL,					NULL},
};
