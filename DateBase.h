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

	bool OpenDataBase(CString dbName="dscsystem",CString userName="root",CString password="123456");	//建立数据库连接
	bool CloseDataBase();	//关闭数据库连接

	bool AdoInitRecordset();//初始化Recordset
	bool AdoInitCommand();//初始化Command
	
	//插入一条数据
	bool InsertData(CString id,CString time, CString data);	//操作成功返回true,失败返回false
	bool InsertData(CString id);
	//删除一条时间为time的数据
	bool DeleteData(CString time);					//操作成功返回true,失败返回false

	//筛选一条时间为time的数据，并保存到data中返回
	bool SelectData(CString time,CString& data);	//操作成功返回true,失败返回false
	bool SelectData(CString id);
	//JC2
	//筛选时间在time_start到time_end之间的数据，并保存到data_arr中,count返回记录条数
	bool SelectData(CString time_start,CString time_end,data_rd* data_arr,int& count);//操作成功返回true,失败返回false

	//返回数据库是否连接 //JC2
	bool isConnected();

	bool CreateTable(CString id);

public:
	_ConnectionPtr m_pConnection;
	_RecordsetPtr m_pRecordset;//执行不带参数的SQL 语句
	_CommandPtr m_pCommand; //执行带参数的SQL 语句
	int m_nID;
	bool m_bConnect;
	
};

#endif // !defined(AFX_DATEBASE_H__3F2FE802_0B48_4C75_AA6D_1BEE3E743062__INCLUDED_)
