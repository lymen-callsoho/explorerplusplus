// Copyright (C) Explorer++ Project
// SPDX-License-Identifier: GPL-3.0-only
// See LICENSE in the top level directory

#include "stdafx.h"
#include "HardwareChangeNotifier.h"

HardwareChangeNotifier& HardwareChangeNotifier::GetInstance()
{
	static HardwareChangeNotifier hcn;
	return hcn;
}

void HardwareChangeNotifier::AddObserver(NHardwareChangeNotifier::INotification *hcn)
{
	m_Observers.push_back(hcn);
}

void HardwareChangeNotifier::RemoveObserver(NHardwareChangeNotifier::INotification *hcn)
{
	auto itr = std::find_if(m_Observers.begin(),m_Observers.end(),
		[hcn](const NHardwareChangeNotifier::INotification *hcnCurrent){return hcnCurrent == hcn;});

	if(itr != m_Observers.end())
	{
		m_Observers.erase(itr);
	}
}

void HardwareChangeNotifier::NotifyDeviceArrival(DEV_BROADCAST_HDR *dbh)
{
	NotifyObservers(NOTIFY_DEVICE_ARRIVAL,dbh);
}

void HardwareChangeNotifier::NotifyDeviceRemovalComplete(DEV_BROADCAST_HDR *dbh)
{
	NotifyObservers(NOTIFY_DEVICE_REMOVAL_COMPLETE,dbh);
}

void HardwareChangeNotifier::NotifyObservers(NotificationType_t NotificationType,DEV_BROADCAST_HDR *dbh)
{
	for(const auto &hcn : m_Observers)
	{
		switch(NotificationType)
		{
		case NOTIFY_DEVICE_ARRIVAL:
			hcn->OnDeviceArrival(dbh);
			break;

		case NOTIFY_DEVICE_REMOVAL_COMPLETE:
			hcn->OnDeviceRemoveComplete(dbh);
			break;
		}
	}
}