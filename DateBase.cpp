// DateBase.cpp: implementation of the CDateBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DSC.h"
#include "DateBase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDateBase::CDateBase()
{
	m_nID=1;
	this->m_bConnect = false;
}

CDateBase::~CDateBase()
{

}


bool CDateBase::OpenDataBase(CString dbName,CString userName,CString password)
{
	::CoInitialize(NULL);
    HRESULT hr = m_pConnection.CreateInstance(__uuidof(Connection));
    if(FAILED(hr))
	{
		AfxMessageBox("数据库连接创建失败！");
	    return false;
    }
	CString str="";
	str = "DRIVER={MySQL ODBC 3.51 Driver};Server=localhost;";
	str = str+"Database="+dbName+";uid="+userName+";pwd="+password+";";
	//AfxMessageBox(str);
    /*连接字符串*/
    //_bstr_t strConnect = "Provider=SQLOLEDB;Persist Security Info=False;User ID=sa;Initial Catalog=BankAccount";
    _bstr_t strConnect = str; 
    try
	{
		/*打开连接*/
		m_pConnection->Open(strConnect,"","",NULL);
    }
    catch(_com_error &e)
	{
 	    /*数据库连接失败*/
	    ::MessageBox(NULL,e.Description(),"警告",MB_OK|MB_ICONWARNING);
		return false;
	}

	this->m_bConnect = true;
	return true;
}

bool CDateBase::CloseDataBase()
{
		if (m_pConnection == NULL) 
	{ 
		//  如果连接已经为空 
		return TRUE; 
	} 
    try 
	{ 
		m_pConnection->Close(); 
		m_pConnection = NULL; 
		return TRUE; 
	} 
	catch(_com_error e) 
	{ 
		TRACE(_T("关闭数据库发生错误：%s\n"), e.ErrorMessage()); 
	} 

	this->m_bConnect = false;
	return true;
}


bool CDateBase::AdoInitRecordset()
{
	HRESULT hr  = m_pRecordset.CreateInstance(__uuidof(Recordset));
	if(FAILED(hr)){
		AfxMessageBox("记录集创建失败！");
	    return false;
	}
	else{
		return true;
	}
}

bool CDateBase::AdoInitCommand()
{
	HRESULT hr  = m_pCommand.CreateInstance(__uuidof(Command));
	if(FAILED(hr)){
		AfxMessageBox("命令集创建失败！");
	    return false;
	}
	try{
		/*将命令对象与连接对象关联起来*/
		m_pCommand->ActiveConnection = m_pConnection;
	}
	catch(_com_error e){
		::MessageBox(NULL,e.Description(),"警告",MB_OK|MB_ICONWARNING);
		return false;
	}
	return true;
}

bool CDateBase::InsertData(CString id,CString time,CString data)
{
	

//	AdoInitCommand();
	CreateTable(id);

	id = "tb_"+id;

   /*具体查询*/
	char uid[50];
	strcpy(uid,id.GetBuffer(0));

	char datatime[50];
	strcpy(datatime,time.GetBuffer(0));

	char content[1024];
	strcpy(content,data.GetBuffer(0));
	
	CString szSql;
	szSql.Format("insert into %s (time,content)values('%s','%s')",uid,datatime,content);
	_variant_t vr; 
    try 
    { 
        m_pConnection->Execute((_bstr_t)szSql,&vr,adExecuteNoRecords); 
    } 
    catch(_com_error&  e)          
    {          
//		AfxMessageBox("错误！");  
        AfxMessageBox(e.Description());  
        //ado.AdoClose();
        return false;          
    } 
//	AfxMessageBox("写入数据库正确");

	return true;
}

bool CDateBase::InsertData(CString id)
{
	char uid[50];
	strcpy(uid,id.GetBuffer(0));
	
	CString szSql;
	szSql.Format("insert into dsc (userid)values('%s')",uid);
	_variant_t vr; 
    try 
    { 
        m_pConnection->Execute((_bstr_t)szSql,&vr,adExecuteNoRecords); 
    } 
    catch(_com_error&  e)          
    {          
		AfxMessageBox("错误！");  
        AfxMessageBox(e.Description());  
        //ado.AdoClose();
        return false;          
    } 
//	AfxMessageBox("写入数据库正确");

	return true;
}

bool CDateBase::DeleteData(CString time)
{
	return true;
}

bool CDateBase::SelectData(CString time,CString& data)
{
	return true;
}


bool CDateBase::SelectData(CString time_start,CString time_end,data_rd* data_arr,int& count)
{
	return true;
}

//JC2
bool CDateBase::isConnected()
{
	return this->m_bConnect;
}


//id="13988889999"
bool CDateBase::CreateTable(CString id)
{
	AdoInitRecordset();
	AdoInitCommand();

	//AfxMessageBox("create..."+id);
	if(true==SelectData(id))
	{
//		AfxMessageBox("表已经存在");
		return true;
	}
	else
	{
//		AfxMessageBox("表bu存在");
		CString szSql;
		CString id1 = "tb_"+id;
		szSql.Format("create table %s (id int auto_increment primary key auto_increment,time datetime not null,content varchar(1024))",id1);

		_variant_t vr; 
		try 
		{ 
			m_pConnection->Execute((_bstr_t)szSql,&vr,adExecuteNoRecords); 
		} 
		catch(_com_error&  e)          
		{          
//			AfxMessageBox("错误！");  
			AfxMessageBox("Create Table Erroe "+e.Description());  
			//ado.AdoClose();
			return false;          
		} 
		//去掉tb_
		//id = id.Mid(3);
		this->InsertData(id);
		//AfxMessageBox("创建表成功");
	}	
	return true;
}

bool CDateBase::SelectData(CString userid)
{
	AdoInitRecordset();
	AdoInitCommand();

//	AfxMessageBox("SelectData"+userid);

    /*具体查询*/
	char use[50];
	strcpy(use,userid.GetBuffer(0));
    
	m_pCommand->CommandText = _bstr_t("select userid from dsc");
	_ParameterPtr Param1;
	Param1 = m_pCommand->CreateParameter(_bstr_t("use"),adChar,adParamInput,sizeof(char)*50,use);
	m_pCommand->Parameters->Append(Param1);
      /*执行查询操作*/
	bool flag=false;   //用来标志 是否存在
	
	CString str="";
	m_pRecordset=m_pCommand->Execute((VARIANT*)NULL,(VARIANT*)NULL,adCmdText);
	while(m_pRecordset->rsEOF== VARIANT_FALSE )
	{
	//		str =m_pRecordset->Fields->GetItem(_variant_t("userid"))->Value;
		_variant_t varName;
		varName = m_pRecordset->GetCollect ("userid");

		str =(char *)_bstr_t(varName);
//		AfxMessageBox("userid"+userid);
//		AfxMessageBox("from table："+str);
		if(str.Compare(userid)==0)
		{
//			AfxMessageBox("查到");
			flag=true;
		}

		m_pRecordset->MoveNext();
	}
	return flag;
}