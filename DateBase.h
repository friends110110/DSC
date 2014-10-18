// DateBase.h: interface for the CDateBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATEBASE_H__3F2FE802_0B48_4C75_AA6D_1BEE3E743062__INCLUDED_)
#define AFX_DATEBASE_H__3F2FE802_0B48_4C75_AA6D_1BEE3E743062__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct
{
	int id;
	CString time;
	CString data;
}data_rd;//JC2

class CDateBase  
{
public:
	CDateBase();
	virtual ~CDateBase();

	bool OpenDataBase(CString dbName="dscsystem",CString userName="root",CString password="123456");	//�������ݿ�����
	bool CloseDataBase();	//�ر����ݿ�����

	bool AdoInitRecordset();//��ʼ��Recordset
	bool AdoInitCommand();//��ʼ��Command
	
	//����һ������
	bool InsertData(CString id,CString time, CString data);	//�����ɹ�����true,ʧ�ܷ���false
	bool InsertData(CString id);
	//ɾ��һ��ʱ��Ϊtime������
	bool DeleteData(CString time);					//�����ɹ�����true,ʧ�ܷ���false

	//ɸѡһ��ʱ��Ϊtime�����ݣ������浽data�з���
	bool SelectData(CString time,CString& data);	//�����ɹ�����true,ʧ�ܷ���false
	bool SelectData(CString id);
	//JC2
	//ɸѡʱ����time_start��time_end֮������ݣ������浽data_arr��,count���ؼ�¼����
	bool SelectData(CString time_start,CString time_end,data_rd* data_arr,int& count);//�����ɹ�����true,ʧ�ܷ���false

	//�������ݿ��Ƿ����� //JC2
	bool isConnected();

	bool CreateTable(CString id);

public:
	_ConnectionPtr m_pConnection;
	_RecordsetPtr m_pRecordset;//ִ�в���������SQL ���
	_CommandPtr m_pCommand; //ִ�д�������SQL ���
	int m_nID;
	bool m_bConnect;
	
};

#endif // !defined(AFX_DATEBASE_H__3F2FE802_0B48_4C75_AA6D_1BEE3E743062__INCLUDED_)
