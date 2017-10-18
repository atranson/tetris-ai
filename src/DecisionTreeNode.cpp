#include "DecisionTreeNode.h"

namespace TetrisAI {

	DecisionTreeNode::NodeStatus DecisionTreeNode::mergeNodesStatus(std::vector<std::unique_ptr<DecisionTreeNode>>& nodes)
	{
		NodeStatus output;
		for (auto& node : nodes)
		{
			NodeStatus status = node->getNodeStatus();
			// Merge overlapping elements
			for (unsigned i = 0; i < output.size() && i < status.size(); i++)
			{
				for (const auto& kv : status[i])
				{
					// output[i][kv.first] += kv.second with test for key existence included
					output[i][kv.first] = kv.second + (output[i].find(kv.first) != output[i].end() ? output[i][kv.first] : 0);
				}
			}
			// Push remaining ones if there are any
			for (unsigned i = output.size(); i < status.size(); i++)
			{
				output.push_back(status[i]);
			}
		}
		return output;
	}

}