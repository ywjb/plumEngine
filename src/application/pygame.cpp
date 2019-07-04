#include "application/pygame.h"
#include "script/ScriptUtils.h"
#include "application/CApplication.h"
#include "application/CAsioService.h"
#include "common/CTime.h"
#include "common/log.h"

namespace application
{
	//�����������
	PyObject* StartNetwork(PyObject* self, PyObject* args);
	//���ӷ�����
	PyObject* ConnectToServer(PyObject* self, PyObject* args);
	//������Ϣ��������
	PyObject* SendMsgToServer(PyObject* self, PyObject* args);
	//������Ϣ���ͻ���
	PyObject* SendMsgToClient(PyObject* self, PyObject* args);
	//��ȡ����ʱ�������λ��
	PyObject* GetFastTimeInSec(PyObject* self, PyObject* args);
	//��ȡ����ʱ�������λ����
	PyObject* GetFastTimeInMs(PyObject* self, PyObject* args);
	//����֡��
	PyObject* InitFrameRate(PyObject* self, PyObject* args);
	//д��־
	PyObject* Log(PyObject* self, PyObject* args);
	//����ڴ�ͳ��
	PyObject* GenMemStat(PyObject* self, PyObject* args);
	//ע��tick
	PyObject* RegTick(PyObject* self, PyObject* args);
	//ɾ��tick
	PyObject* DelTick(PyObject* self, PyObject* args);
	//��������������
	PyObject* GenCrash(PyObject* self, PyObject* args);
	//����Ӧ�ò��������
	PyObject* OpenHeartBeat(PyObject* self, PyObject* args);
	//���������־����
	PyObject* SetLogLevel(PyObject* self, PyObject* args);

	static PyMethodDef Scripting_Methods[] =
	{
		{ "StartNetwork", (PyCFunction)StartNetwork, METH_VARARGS, "�����������" },
		{ "ConnectToServer", (PyCFunction)ConnectToServer, METH_VARARGS, "���ӷ�����" },
		{ "SendMsgToServer", (PyCFunction)SendMsgToServer, METH_VARARGS, "������Ϣ��ָ���ķ���������" },
		{ "SendMsgToClient", (PyCFunction)SendMsgToClient, METH_VARARGS, "������Ϣ��ָ���Ŀͻ�������" },
		{ "GetFastTimeInSec", (PyCFunction)GetFastTimeInSec, METH_NOARGS, "��ȡ����ʱ�������λ��" },
		{ "GetFastTimeInMs", (PyCFunction)GetFastTimeInMs, METH_NOARGS, "��ȡ����ʱ�������λ����" },
		{ "InitFrameRate", (PyCFunction)InitFrameRate, METH_VARARGS, "��ʼ��֡��" },
		{ "Log", (PyCFunction)Log, METH_VARARGS, "д��־" },
		{ "GenMemStat", (PyCFunction)GenMemStat, METH_NOARGS, "��ӡ�ڴ�ͳ����Ϣ" },
		{ "RegTick", (PyCFunction)RegTick, METH_VARARGS, "ע��tick" },
		{ "DelTick", (PyCFunction)DelTick, METH_VARARGS, "ɾ��tick" },
		{ "GenCrash", (PyCFunction)GenCrash, METH_VARARGS, "�ű���������������" },
		{ "OpenHeartBeat", (PyCFunction)OpenHeartBeat, METH_VARARGS, "����Ӧ�ò��������" },
		{ "SetLogLevel", (PyCFunction)SetLogLevel, METH_VARARGS, "���������־����" },
		{ NULL, NULL }
	};

	//�ű����ƿ�������
	PyObject* StartNetwork(PyObject *self, PyObject *args)
	{
		int32 nettype = 0;
		char* szIP = 0;
		int32 port = 0;
		uint16 limit_msg_num = 0;
		uint8 limit_invalid_num = 0;
		bool encrypt = false;
		int32 maxclients = 5000;
		if (!PyArg_ParseTuple(args, "isiHB|Bi", &nettype, &szIP, &port, &limit_msg_num, &limit_invalid_num, &encrypt, &maxclients))
			return NULL;

		CApplication::GetInstance()->StartNetwork(nettype, szIP, port, limit_msg_num, limit_invalid_num, encrypt, maxclients);

		PY_RETURN_TRUE;
	}

	//�ű��������ӷ�����
	PyObject* ConnectToServer(PyObject *self, PyObject *args)
	{
		int32 nettype = 0;
		char* szAddr = 0;
		int32 port = 0;
		bool encrypt = false;
		int32 retrytimes = 0;
		int32 retrysec = 5;
		if (!PyArg_ParseTuple(args, "isi|iiB", &nettype, &szAddr, &port, &retrytimes, &retrysec, &encrypt))
			return NULL;

		CApplication::GetInstance()->ConnectToServer(nettype, szAddr, port, encrypt, retrytimes, retrysec);

		PY_RETURN_TRUE;
	}

	//�ű��������ӷ�����
	PyObject* SendMsgToClient(PyObject *self, PyObject *args)
	{
		int32 nettype = 0;
		uint32 pid = 0;
		uint8 proto_type = 0;
		char* msg = 0;
		uint32 uSize = 0;
		if (!PyArg_ParseTuple(args, "iIBs#", &nettype, &pid, &proto_type, &msg, &uSize))
			return NULL;

		if (msg == NULL || uSize == 0){
			PY_RETURN_FALSE;
		}

		CApplication::GetInstance()->GetAsioService()->SendToClient(nettype, pid, proto_type, msg, uSize);

		PY_RETURN_TRUE;
	}

	//�ű��������ӷ�����
	PyObject* SendMsgToServer(PyObject *self, PyObject *args)
	{
		int32 nettype = 0;
		uint8 proto_type = 0;
		char* msg = 0;
		uint32 uSize = 0;
		if (!PyArg_ParseTuple(args, "iBs#", &nettype, &proto_type, &msg, &uSize))
			return NULL;

		if (msg == NULL || uSize == 0){
			PY_RETURN_FALSE;
		}

		CApplication::GetInstance()->GetAsioService()->SendToServer(nettype, proto_type, msg, uSize);

		PY_RETURN_TRUE;
	}

	PyObject* GetFastTimeInSec(PyObject* self, PyObject* args)
	{
		return PyLong_FromUnsignedLongLong(common::GetFastUTCTimeInSec());
	}

	PyObject* GetFastTimeInMs(PyObject* self, PyObject* args)
	{
		return PyLong_FromUnsignedLongLong(common::GetFastUTCTimeInMS());
	}

	PyObject* GenMemStat(PyObject* self, PyObject* args)
	{
		CApplication::GetInstance()->GenMemStat();
		PY_RETURN_TRUE;
	}

	//��ʼ��֡��
	PyObject* InitFrameRate(PyObject *self, PyObject *args)
	{
		uint8 uFrameRate = 0;
		if (!PyArg_ParseTuple(args, "B", &uFrameRate))
			return NULL;
		CApplication::GetInstance()->InitFrameRate(uFrameRate);
		PY_RETURN_TRUE;
	}

	//�ű�д��־
	PyObject* Log(PyObject *self, PyObject *args)
	{
		int32 logtype = 0;
		char* szMsg = 0;
		char* szMod = 0;
		if (!PyArg_ParseTuple(args, "iss", &logtype, &szMsg, &szMod))
			return NULL;

		common::LogMod((common::LogLevel)logtype, szMod, "script | %s", szMsg);

		PY_RETURN_TRUE;
	}

	//ע��ű�tick
	PyObject* RegTick(PyObject *self, PyObject *args)
	{
		int nCnt = -1;
		uint32 uInterval = 0;
		PyObject* pArgs = NULL;
		PyObject* pCallBack = NULL;
		if (!PyArg_ParseTuple(args, "OOI|i", &pCallBack, &pArgs, &uInterval, &nCnt))
			return NULL;
		CApplication::GetInstance()->RegScriptTick(pCallBack, pArgs, uInterval, nCnt);
		PY_RETURN_TRUE;
	}

	//ɾ���ű�tick
	PyObject* DelTick(PyObject *self, PyObject *args)
	{
		PyObject* pCallBack = NULL;
		if (!PyArg_ParseTuple(args, "O", &pCallBack))
			return NULL;
		CApplication::GetInstance()->DelScriptTick(pCallBack);
		PY_RETURN_OK;
	}

	PyObject* GenCrash(PyObject *self, PyObject *args)
	{
		char* szMsg = 0;
		if (!PyArg_ParseTuple(args, "s", &szMsg))
			return NULL;
		CApplication::GetInstance()->GenErr(szMsg);
		PY_RETURN_OK;
	}

	//�ű��������ӷ�����
	PyObject* OpenHeartBeat(PyObject *self, PyObject *args)
	{
		int32 sendTick = 0;
		int32 checkTick = 0;
		if (!PyArg_ParseTuple(args, "ii", &sendTick, &checkTick))
			return NULL;
		CApplication::GetInstance()->OpenHeartBeat(sendTick, checkTick);
		PY_RETURN_OK;
	}

	//�ű�������־������
	PyObject* SetLogLevel(PyObject *self, PyObject *args)
	{
		int32 logtype = 0;
		if (!PyArg_ParseTuple(args, "i", &logtype))
			return NULL;

		common::SetLogLevel((common::LogLevel)logtype);

		PY_RETURN_TRUE;
	}

	PyObject* PyGame_Init()
	{
		Py_InitModule((char*) "game", Scripting_Methods);
		script::AddIntConstToModule("LOG_LV_DEBUG", LOG_LV_DEBUG);
		script::AddIntConstToModule("LOG_LV_INFO", LOG_LV_INFO);
		script::AddIntConstToModule("LOG_LV_WARNING", LOG_LV_WARNING);
		script::AddIntConstToModule("LOG_LV_ERROR", LOG_LV_ERROR);
		return script::GetImportedModule("game");
	}
}