#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <unordered_map>
#include <regex>
#include <algorithm>
#include <memory>

struct	Wire
{
	Wire() = default;
	Wire(std::string id, int val = -1):
		identifier(id),
		value(val)
		{ }

	bool	operator<(const Wire& rhs) const
	{
		return this->identifier < rhs.identifier;
	}

	const std::string	identifier;
	int					value; // -1 for no value, 0 or 1 otherwise
};

template<class T>
using sp = std::shared_ptr<T>;

template<class T>
using up = std::unique_ptr<T>;

struct Gate
{
	Gate() = default;
	Gate(sp<Wire> in1, sp<Wire> in2, sp<Wire> out1)
	{
		inputs[0] = in1;
		inputs[1] = in2;
		output = out1;
	}
	virtual ~Gate() = default;

	sp<Wire>	inputs[2];
	sp<Wire>	output;
	virtual void	do_op() const = 0;
};

struct AND_Gate final : public Gate
{
	using Gate::Gate;
	void	do_op() const override
	{
		const int& o1 = inputs[0]->value;
		const int& o2 = inputs[1]->value;
		output->value = o1 & o2;
	}
};

struct OR_Gate final : public Gate
{
	using Gate::Gate;
	void	do_op() const override
	{
		const int& o1 = inputs[0]->value;
		const int& o2 = inputs[1]->value;
		output->value = o1 | o2;
	}
};

struct XOR_Gate final : public Gate
{
	using Gate::Gate;
	void	do_op() const override
	{
		const int& o1 = inputs[0]->value;
		const int& o2 = inputs[1]->value;
		output->value = o1 ^ o2;
	}
};

template<typename T>
void	print(const T& t)
{
	std::cout << t << '\n';
}

std::vector<up<Gate>>	parse(const std::string& filename)
{
	std::unordered_map<std::string,sp<Wire>>	wire_map {};
	std::ifstream			file {filename};
	std::string				line {};
	std::vector<up<Gate>>	gates {};

	std::regex			pattern {R"((.{3}): (\d))"};
	for (std::getline(file, line);
		!line.empty();
		std::getline(file, line))
	{
		std::smatch	match;
		std::regex_match(line, match, pattern);
		const std::string&	wire = match[1].str();
		const int&			initial_value = std::stoi(match[2]);
		wire_map[wire] = std::make_shared<Wire>(wire, initial_value);
	}

	pattern = R"((.{3}) (AND|OR|XOR) (.{3}) -> (.{3}))";
	while (std::getline(file, line))
	{
		std::smatch	match;
		std::regex_match(line, match, pattern);
		const std::string&	in_wire1 = match[1].str();
		const std::string&	gate_type = match[2].str();
		const std::string&	in_wire2 = match[3].str();
		const std::string&	out_wire = match[4].str();

		if (!wire_map.contains(in_wire1))
		{
			wire_map[in_wire1] = std::make_shared<Wire>(in_wire1);
		}
		if (!wire_map.contains(in_wire2))
		{
			wire_map[in_wire2] = std::make_shared<Wire>(in_wire2);
		}
		if (!wire_map.contains(out_wire))
		{
			wire_map[out_wire] = std::make_shared<Wire>(out_wire);
		}
		sp<Wire>	in1 = wire_map[in_wire1];
		sp<Wire>	in2 = wire_map[in_wire2];
		sp<Wire>	out1 =  wire_map[out_wire];
		if (gate_type == "AND")
		{
			gates.push_back(std::make_unique<AND_Gate>(in1, in2, out1));
		}
		else if (gate_type == "OR")
		{
			gates.push_back(std::make_unique<OR_Gate>(in1, in2, out1));
		}
		else
		{
			gates.push_back(std::make_unique<XOR_Gate>(in1, in2, out1));
		}
	}
	return gates;
}

bool	has_output_awaiting(std::vector<up<Gate>>& gates)
{
	for (auto it = gates.begin(); it != gates.end(); ++it)
	{
		if (it->get()->inputs[0]->value == -1 ||
			it->get()->inputs[1]->value == -1 ||
			it->get()->output->value == -1)
		{
			return true;
		}
	}
	return false;
}

int	main()
{
	std::vector<up<Gate>>	gates = parse("input.txt");

	while (has_output_awaiting(gates))
	{
		for (auto it = gates.begin(); it != gates.end(); ++it)
		{
			Gate*		gate = it->get();
			sp<Wire>	in1 = gate->inputs[0];
			sp<Wire>	in2 = gate->inputs[1];

			if (in1->value == -1 || in2->value == -1) continue;
			gate->do_op();
		}
	}

	std::set<Wire>	z_wires {};
	for (auto it = gates.begin(); it != gates.end(); ++it)
	{
		sp<Wire>	outwire = it->get()->output;
		if (outwire->identifier[0] != 'z') continue ;
		z_wires.insert(*it->get()->output);
	}

	uint64_t	number {0};
	uint64_t	place_val {1};
	for (auto it = z_wires.begin(); it != z_wires.end(); ++it)
	{
		number += it->value * place_val;
		place_val <<= 1;
	}
	std::cout << "The number produced is: " << number << '\n';
}