 /* ========================================================================
  * Copyright (C) 2001  Vienna University of Technology
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Lesser General Public
  * License as published by the Free Software Foundation; either
  * version 2.1 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Lesser General Public License for more details.
  *
  * You should have received a copy of the GNU Lesser General Public
  * License along with this library; if not, write to the Free Software
  * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  * For further information please contact Gerhard Reitmayr under
  * <reitmayr@ims.tuwien.ac.at> or write to Gerhard Reitmayr,
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
  * Austria.
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** source file for the Ubisense interface module.
  *
  * @author Mathis Csisinko
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

// this will remove the warning 4786
#include "../tool/disable4786.h"

#include "UbisenseModule.h"
#include "UbisenseSource.h"

#ifdef USE_UBISENSE

#include "UClientAPI/name_client.h"

#ifdef WIN32
#pragma comment(lib,"UClientAPI.lib")
#endif

namespace ot {

UbisenseModule::UbisenseModule(): Module(),NodeFactory(),locationClient(),dataClient(pSources),cells(locationClient.get_all_cells())
{
}

UbisenseModule::~UbisenseModule()
{
    for (NodeVector::iterator it = pSources.begin();it != pSources.end();it ++)
        delete *it;
    pSources.clear();
}

Node* UbisenseModule::createNode(const std::string &name,StringTable &attributes)
{
    if (name.compare("UbisenseSource") == 0)
    {
		NameClient nameClient;
		Object object;
		if (nameClient.get_named_object(attributes.get("object").c_str(),object))
		{
			UbisenseSource* pSource = new UbisenseSource(object,locationClient,dataClient);
			pSources.push_back(pSource);
	        return pSource;
		}
    }
    return NULL;
}

void UbisenseModule::start()
{
	for (Set<String>::const_iterator i = cells.begin();i != cells.end();++ i)
		locationClient.load_cell(*i);
}

void UbisenseModule::close()
{
	locationClient.unload_all_cells();
}

void UbisenseModule::pushState()
{
    for (NodeVector::iterator it = pSources.begin();it != pSources.end();it ++)
    {
        UbisenseSource* pSource = reinterpret_cast<UbisenseSource*>(*it);
		if (pSource->calcState())
			pSource->updateObservers(pSource->getState());
    }
}

void UbisenseModule::init(StringTable &attributes,ConfigNode* pLocalTree)
{
	NameClient nameClient;
	Object object;
	cells.clear();
	for (unsigned int i = 0;i < pLocalTree->countChildren();i ++)
	{
		ConfigNode* pNode = reinterpret_cast<ConfigNode*>(pLocalTree->getChild(i));
		if (! pNode->getType().compare("UbisenseCell"))
			cells.insert(pNode->getAttributes().get("name").c_str());
	}
}


UbisenseModule::WrappedDataClient::WrappedDataClient(NodeVector &pSources): DataClient(),pSources(pSources)
{
}

void UbisenseModule::WrappedDataClient::on_button(const Object &tag,UbitagButton button,double time)
{
	Object owner;
	get_tag_owner(tag,owner);
    for (NodeVector::iterator it = pSources.begin();it != pSources.end();it ++)
    {
        UbisenseSource* pSource = reinterpret_cast<UbisenseSource*>(*it);
		const Object &object = pSource->getObject();
		if (tag == object || owner == object)
			pSource->setButtonState(button == Orange ? 0x0001: button == Blue ? 0x0002: 0x0000,time);
	}
}

} // namespace ot

#endif  // USE_UBISENSE
