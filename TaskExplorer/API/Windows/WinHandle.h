#pragma once
#include "../HandleInfo.h"

// begin_phapppub
#define PH_HANDLE_FILE_SHARED_READ 0x1
#define PH_HANDLE_FILE_SHARED_WRITE 0x2
#define PH_HANDLE_FILE_SHARED_DELETE 0x4
#define PH_HANDLE_FILE_SHARED_MASK 0x7

class CWinHandle : public CHandleInfo
{
	Q_OBJECT
public:
	CWinHandle(QObject *parent = nullptr);
	virtual ~CWinHandle();

	virtual quint64 GetObject()	const				{ QReadLocker Locker(&m_Mutex); return m_Object; }
	virtual ulong GetAttributes() const				{ QReadLocker Locker(&m_Mutex); return m_Attributes; }
	virtual ulong GetGrantedAccess() const			{ QReadLocker Locker(&m_Mutex); return m_GrantedAccess; }
	virtual ulong GetTypeIndex() const				{ QReadLocker Locker(&m_Mutex); return m_TypeIndex; }
	virtual ulong GetFileFlags() const				{ QReadLocker Locker(&m_Mutex); return m_FileFlags; }

	virtual QString GetOriginalName() const			{ QReadLocker Locker(&m_Mutex); return m_OriginalName; }

	virtual QString GetAttributesString() const;
	virtual QString GetFileShareAccessString() const;
	virtual QString GetTypeString() const;
	virtual QString GetGrantedAccessString() const;

	virtual QVariantMap GetDetailedInfos() const;

protected:
	friend class CWindowsAPI;
	friend class CWinProcess;

	bool InitStaticData(struct _SYSTEM_HANDLE_TABLE_ENTRY_INFO_EX* handle);

	bool InitExtData(struct _SYSTEM_HANDLE_TABLE_ENTRY_INFO_EX* handle, quint64 ProcessHandle) { return InitExtData(handle, ProcessHandle, true); }
	QFutureWatcher<bool>* InitExtDataAsync(struct _SYSTEM_HANDLE_TABLE_ENTRY_INFO_EX* handle, quint64 ProcessHandle);

	bool UpdateDynamicData(struct _SYSTEM_HANDLE_TABLE_ENTRY_INFO_EX* handle, quint64 ProcessHandle);

	quint64				m_Object;
	ulong				m_Attributes;
	ulong				m_GrantedAccess;
	ulong				m_TypeIndex;
	ulong				m_FileFlags;

	QString				m_TypeName;
    QString				m_OriginalName;

	QString				m_SubTypeName;

private:
	bool InitExtData(struct _SYSTEM_HANDLE_TABLE_ENTRY_INFO_EX* handle, quint64 ProcessHandle, bool bFull);
	static bool InitExtDataAsync(CWinHandle* This, struct _SYSTEM_HANDLE_TABLE_ENTRY_INFO_EX* handle, quint64 ProcessHandle);

};