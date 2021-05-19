#include <iostream>
#include <vector>

#include <elfio/elfio.hpp>

int main(int argc, char* argv[])
{
	std::vector<std::string> args(argv, argv + argc);

	if (args.size() <= 1)
	{
		std::cerr << "Provide an ELF path" << std::endl;
	}

	ELFIO::elfio reader;

	if (!reader.load(args[1].c_str()))
	{
		std::cerr << "Couldn't load ELF at " << args[1] << std::endl;
	}

	Elf_Half sec_num = reader.sections.size();
	std::cout << "Number of sections: " << sec_num << std::endl;
	for (int i = 0; i < sec_num; ++i)
	{
		const section* psec = reader.sections[i];
		std::cout << "  [" << i << "] " << psec->get_name() << "\t" << psec->get_size() << std::endl;// Access section's dataconst char* p = reader.sections[i]->get_data();}

		return 0;
	}
