/*!
 * \file qiphelper.cpp
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
#include "stdafx.h"
#include "QIPHelper.h"

//#ifndef WIN32_LEAN_AND_MEAN
//#define WIN32_LEAN_AND_MEAN
//#endif

#include <windows.h>
//#include <winsock2.h>
#include <iphlpapi.h>


QIPHelper::QIPHelper(QObject *parent)
	: QObject(parent)
{

}

QIPHelper::~QIPHelper()
{
}

/*!
 * \brief	
 *
 * \param	
 * \return	
 * \note
 */
ip_adapter_info_t const &QIPHelper::get_adapter_info(int index)
{
	IP_ADAPTER_INFO *padapter_info;
	ULONG			out_buf_len;
	DWORD			res;

	padapter_info = (IP_ADAPTER_INFO *)malloc(sizeof(IP_ADAPTER_INFO));
	out_buf_len = sizeof(IP_ADAPTER_INFO);

	// make an initial call to get the size needed into the out_buf_len variable.
	// This call to the function is meant to fail, and is used to ensure that the out_buf_len variable specifies a size
	// sufficient for holding all the information returned to 'padapter_info'. This is a common programming model for data
	// structures and functions of this type.
	if ( GetAdaptersInfo( padapter_info, &out_buf_len) != ERROR_SUCCESS)
	{
		free(padapter_info);
		padapter_info = (IP_ADAPTER_INFO *)malloc(out_buf_len);
	}

	if ((res = GetAdaptersInfo(padapter_info, &out_buf_len)) != ERROR_SUCCESS)
	{
		qDebug() << "GetAdaptersInfo call failed with " << res;
	}

	// If the call was successful, access some of the data in the padapter_info structure
	IP_ADAPTER_INFO *padapter = padapter_info;
	
	while (padapter)
	{
		if (padapter->Index == index)
		{
			this->adapter_info.adapter_name = padapter->AdapterName;
			this->adapter_info.description = padapter->Description;
			
			// MAC
			for (int  i = 0; i < padapter->AddressLength; i++)
			{
				if (i == padapter->AddressLength - 1)
				{
					this->adapter_info.address.append(QString().sprintf("%.2X\n", (int)padapter->Address[i]));
				}
				else
				{
					this->adapter_info.address.append(QString().sprintf("%.2X:", (int)padapter->Address[i]));
				}
			}

			// IP address
			this->adapter_info.ip_addr = padapter->IpAddressList.IpAddress.String;
			// IP mask
			this->adapter_info.ip_mask = padapter->IpAddressList.IpMask.String;
			// Gateway
			this->adapter_info.gateway = padapter->GatewayList.IpAddress.String;
			// DHCP
			this->adapter_info.dhcp_enabled = padapter->DhcpEnabled;
			if (padapter->DhcpEnabled)
			{
				this->adapter_info.dhcp_server = padapter->DhcpServer.IpAddress.String;
			}
			else
			{
				this->adapter_info.dhcp_server = "";
			}
		}

		padapter = padapter->Next;
	}

	if (padapter_info)
	{
		free(padapter_info);
	}

	return this->adapter_info;
}
