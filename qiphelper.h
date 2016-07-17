/*!
 * \file qiphelper.h
 * \date 2016/07/13 18:38
 *
 * \author diverger
 *
 * \brief A simple class get the various IP informations from the given network interface(usually a physical adapter) index.
 *
 * TODO: long description
 *
 * \note
 */
#pragma once
#ifndef qiphelper_h__
#define qiphelper_h__
#include <QObject>

typedef struct _ip_adapter_info
{
	QString     adapter_name;
	QString		description;
	QString		address;				// The hardware address for the adapter
	uint32_t	index;
	uint32_t	type;
	uint32_t	dhcp_enabled;
	QString		current_ip_address;
	QString		ip_addr;
	QString		ip_mask;
	QString		gateway;
	QString		dhcp_server;
} ip_adapter_info_t, *ip_adapter_info_p;

class QIPHelper : public QObject
{
	Q_OBJECT
public:
	QIPHelper(QObject *parent = Q_NULLPTR);
	~QIPHelper();

public:	// public functions
	ip_adapter_info_t const &get_adapter_info(int index);

private:	// private members
	
	ip_adapter_info_t adapter_info;
};

#endif // qiphelper_h__
