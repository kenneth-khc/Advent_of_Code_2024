#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <vector>
#include <regex>
#include <cmath>

const int	NUM_REGISTERS = 3;

using Registers = std::array<int, NUM_REGISTERS>;
using Instructions = std::vector<int>;
using std::size_t;

enum Register
{
	A = 0,
	B = 1,
	C = 2
};

enum class Op_Codes
{
	adv = 0,
	bxl = 1,
	bst = 2,
	jnz = 3,
	bxc = 4,
	out = 5,
	bdv = 6,
	cdv = 7
};

int	get_combo_val(Registers& regs, int& combo_operand)
{
	if (combo_operand >= 0 && combo_operand <= 3)
	{
		return combo_operand;
	}
	else
	{
		return regs[combo_operand % 4];
	}
}

void	adv(Registers& regs, int& operand)
{
	int	numerator = regs[Register::A];
	int	denominator = std::pow(2, get_combo_val(regs, operand));
	regs[Register::A] = numerator / denominator;
}

void	bxl(Registers& regs, int& operand)
{
	regs[Register::B] = regs[Register::B] ^ operand;
}

void	bst(Registers& regs, int& operand)
{
	regs[Register::B] = get_combo_val(regs, operand) % 8;
}

void	jnz([[maybe_unused]] Registers& regs, int& operand, size_t& ip)
{
	ip = operand;
}

void	bxc(Registers& regs, [[maybe_unused]] int& operand)
{
	regs[Register::B] = regs[Register::B] ^ regs[Register::C];
}

void	out(Registers& regs, int& operand, std::vector<int>& output)
{
	int	outval = get_combo_val(regs, operand) % 8;
	output.push_back(outval);
}

void	bdv(Registers& regs, int& operand)
{
	int	numerator = regs[Register::A];
	int	denominator = std::pow(2, get_combo_val(regs, operand));
	regs[Register::B] = numerator / denominator;
}

void	cdv(Registers& regs, int& operand)
{
	int	numerator = regs[Register::A];
	int	denominator = std::pow(2, get_combo_val(regs, operand));
	regs[Register::C] = numerator / denominator;
}

void	get_input(const std::string&, Registers&, Instructions&);

int	main()
{
	Registers			registers;
	Instructions		instructions;
	std::vector<int>	output;

	get_input("input.txt", registers, instructions);

	for (size_t ip = 0; ip < instructions.size();)
	{
		if (static_cast<Op_Codes>(instructions[ip]) == Op_Codes::adv)
			adv(registers, instructions[ip+1]);
		else if (static_cast<Op_Codes>(instructions[ip]) == Op_Codes::bxl)
			bxl(registers, instructions[ip+1]);
		else if (static_cast<Op_Codes>(instructions[ip]) == Op_Codes::bst)
			bst(registers, instructions[ip+1]);
		else if (static_cast<Op_Codes>(instructions[ip]) == Op_Codes::jnz)
		{
			if (registers[Register::A] != 0)
			{
				jnz(registers, instructions[ip+1], ip);
				continue ;
			}
		}
		else if (static_cast<Op_Codes>(instructions[ip]) == Op_Codes::bxc)
			bxc(registers, instructions[ip+1]);
		else if (static_cast<Op_Codes>(instructions[ip]) == Op_Codes::out)
			out(registers, instructions[ip+1], output);
		else if (static_cast<Op_Codes>(instructions[ip]) == Op_Codes::bdv)
			bdv(registers, instructions[ip+1]);
		else if (static_cast<Op_Codes>(instructions[ip]) == Op_Codes::cdv)
			cdv(registers, instructions[ip+1]);
		ip += 2;
		std::cout << "RegA: " << registers[Register::A]
				  << " | RegB: " << registers[Register::B]
				  << " | RegC: " << registers[Register::C]
				  << std::endl;
	}
	std::cout << "Output:\n";
	for (auto it = output.begin(); it != output.end(); ++it)
	{
		std::cout << *it;
		if (it != output.end() - 1)
		{
			std::cout << ",";
		}
	}
	std::cout << std::endl;
}

void	get_input(const std::string& filename, Registers& regs, Instructions& instrucs)
{
	std::ifstream	file {filename};
	std::string		line {};
	std::regex		pattern {R"(\d+)"};
	std::smatch		match;

	for (int reg = 0; reg < NUM_REGISTERS; ++reg)
	{
		std::getline(file, line);
		std::regex_search(line, match, pattern);
		regs[reg] = std::stoi(match[0]);
	}

	std::getline(file, line);
	std::getline(file, line);

	std::sregex_iterator	it {line.begin(), line.end(), pattern};
	std::sregex_iterator	end {};
	while (it != end)
	{
		instrucs.push_back(std::stoi(it->str()));
		++it;
	}
}
