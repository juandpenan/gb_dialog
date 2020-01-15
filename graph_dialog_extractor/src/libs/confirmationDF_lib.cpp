/*********************************************************************
*  Software License Agreement (BSD License)
*
*   Copyright (c) 2018, Intelligent Robotics
*   All rights reserved.
*
*   Redistribution and use in source and binary forms, with or without
*   modification, are permitted provided that the following conditions
*   are met:

*    * Redistributions of source code must retain the above copyright
*      notice, this list of conditions and the following disclaimer.
*    * Redistributions in binary form must reproduce the above
*      copyright notice, this list of conditions and the following
*      disclaimer in the documentation and/or other materials provided
*      with the distribution.
*    * Neither the name of Intelligent Robotics nor the names of its
*      contributors may be used to endorse or promote products derived
*      from this software without specific prior written permission.

*   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*   FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
*   COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
*   INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*   BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
*   CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
*   ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*   POSSIBILITY OF SUCH DAMAGE.
*********************************************************************/

/* Author: Jonatan Gines jginesclavero@gmail.com */

/* Mantainer: Jonatan Gines jginesclavero@gmail.com */
#include <graph_dialog_extractor/confirmationDF_lib.h>
#include <string>
#include <list>

namespace graph_dialog_extractor
{
ConfirmationDF::ConfirmationDF(std::string intent):
  DialogInterface(intent), edge_()
{
  intent_ = intent;
}

void ConfirmationDF::listenCallback(dialogflow_ros_msgs::DialogflowResult result)
{
  if (edge_ == NULL)
    return;

  ROS_INFO("[ConfirmationDF] listenCallback: intent %s", result.intent.c_str());
  graph_.remove_edge(*edge_);
  if (result.intent == "positive.confirmation")
  {
    graph_.add_edge(edge_->get_target(), "response: yes" , edge_->get_source());
  }
  else if (result.intent == "negative.confirmation")
  {
    graph_.add_edge(edge_->get_target(), "response: no" , edge_->get_source());
  }
  edge_ = NULL;
}

void ConfirmationDF::step()
{
  std::list<bica_graph::StringEdge> edges_list =  graph_.get_string_edges();
  for (auto it = edges_list.begin(); it != edges_list.end(); ++it)
  {
    std::string edge = it->get();
    if (edge.find("ask: order.confirmation") != std::string::npos)
    {
      edge_ = new bica_graph::StringEdge(*it);
      ROS_INFO("[confirmationDF Ask] %s", edge.c_str());
      listen();
    }
  }
}

};  // namespace graph_dialog_extractor
