#include <fstream>
#include <iostream>
#include <set>
#include <map>
#include <vector>
#include <string>
#include <regex>

struct Node
{
	std::string		identifier;
	std::set<Node>	connected_to;
	int				connections = 0;

	Node() = default;
	Node(std::string id):
	identifier(id) { };

	Node(std::string id, std::string connected_to_id):
	identifier(id)
	{
		connected_to.insert(Node(connected_to_id));
		++connections;
	}

	Node(std::string id, Node new_connection):
	identifier(id)
	{
		connected_to.insert(new_connection);
		++connections;
	}

	void	add(std::string new_connection_id)
	{
		connected_to.insert(new_connection_id);
		++connections;
	}

	bool	operator<(const Node& rhs) const { return this->identifier < rhs.identifier; }
};

void	find_pcs(std::unordered_map<std::string,Node>& nodes)
{
	int							sets_of_3_pcs_with_t {0};
	std::vector<std::string>	groups_found;

	for (auto it = nodes.begin(); it != nodes.end(); ++it)
	{
		if (it->first[0] != 't')
			continue;

		Node&	t = it->second;

		for (auto itt = t.connected_to.begin(); itt != t.connected_to.end(); ++itt)
		{
			const std::string&	second_id = itt->identifier;
			Node&	tt = nodes[second_id];

			for (auto ittt = tt.connected_to.begin(); ittt != tt.connected_to.end(); ++ittt)
			{
				const std::string&	third_id = ittt->identifier;
				Node&	ttt = nodes[third_id];

				for (auto itttt = ttt.connected_to.begin(); itttt != ttt.connected_to.end(); ++itttt)
				{
					if (itttt->identifier == t.identifier)
					{
						std::vector<std::string>	ids {t.identifier, tt.identifier, ttt.identifier};
						std::sort(ids.begin(), ids.end());
						std::string	tmp;
						for (auto& id : ids)
						{
							tmp.append(id);
						}
						if (std::find(groups_found.begin(), groups_found.end(), tmp) == groups_found.end())
						{
							groups_found.push_back(tmp);
							++sets_of_3_pcs_with_t;
						}
					}
				}
			}
		}
	}
	std::cout << sets_of_3_pcs_with_t << " sets of computers that contains a PC starting with a 't'\n";
}

int	main(int argc, char** argv)
{
	if (argc != 2)
		return 1;

	std::ifstream			file {argv[1]};
	std::regex				pattern {R"((\w+)-(\w+))"};
	std::string				line;
	std::unordered_map<std::string,Node>	nodes;
	std::set<std::string>	encountered;
	while (file >> line)
	{
		std::smatch	match;
		std::regex_match(line, match, pattern);
		if (match.ready())
		{
			std::string	pc1 = match[1];
			std::string	pc2 = match[2];
			if (!encountered.contains(pc1) && !encountered.contains(pc2))
			{
				nodes[pc1] = Node(pc1, pc2);
				nodes[pc2] = Node(pc2, pc1);
			}
			else if (encountered.contains(pc1) && !encountered.contains(pc2))
			{
				nodes[pc1].add(pc2);
				nodes[pc2] = Node(pc2, pc1);
			}
			else if (encountered.contains(pc2) && !encountered.contains(pc1))
			{
				nodes[pc2].add(pc1);
				nodes[pc1] = Node(pc1, pc2);
			}
			else
			{
				nodes[pc1].add(pc2);
				nodes[pc2].add(pc1);
			}
			encountered.insert(pc1);
			encountered.insert(pc2);
		}
	}
	find_pcs(nodes);
}