#include <iostream>
#include <vector>

#include <elfio/elfio.hpp>

struct HDR {
	short magic;                                                                                         /* to verify validity of the table */
	short vstamp;                                                                                        /* version stamp */
	long ilineMax;                                                                                       /* number of line number entries */
	long cbLine;                                                                                         /* number of bytes for line number entries */
	long cbLineOffset;                                                                                   /* offset to start of line number entries*/
	long idnMax;                                                                                         /* max index into dense number table */
	long cbDnOffset;                                                                                     /* offset to start dense number table */
	long ipdMax;                                                                                         /* number of procedures */
	long cbPdOffset;                                                                                     /* offset to procedure descriptor table */
	long isymMax;                                                                                        /* number of local symbols */
	long cbSymOffset;                                                                                    /* offset to start of local symbols*/
	long ioptMax;                                                                                        /* max index into optimization symbol entries */
	long cbOptOffset;                                                                                    /* offset to optimization symbol entries */
	long iauxMax;                                                                                        /* number of auxiliary symbol entries */
	long cbAuxOffset;                                                                                    /* offset to start of auxiliary symbol entries*/
	long issMax;                                                                                         /* max index into local strings */
	long cbSsOffset;                                                                                     /* offset to start of local strings */
	long issExtMax;                                                                                      /* max index into external strings */
	long cbSsExtOffset;                                                                                  /* offset to start of external strings */
	long ifdMax; /* number of file descriptor entries */ /* ifd’s for the file run from 0 to ifdMax-1 */ /* Each one is an index to a CFDR recordindexed off of PCHDR pointer*/
	long cbFdOffset;                                                                                     /* offset to file descriptor table */
	long crfd;                                                                                           /* number of relative file descriptor entries */
	long cbRfdOffset;                                                                                    /* offset to relative file descriptor table */
	long iextMax;                                                                                        /* max index into external symbols */
	long cbExtOffset; /* offset to start of external symbol entries*/                                    /* If you add machine dependent fields, add them here */
};

struct ECOFF_FILE_HDR {
	/** magic number */
	short f_magic;
	/** number of sections */
	short f_nscns;
	/** time and date stamp */
	long f_timdat;
	/** file polonger to symbol table */
	long f_symptr;
	/** number of entries in symbol table */
	long f_nsyms;
	/** size of optional header */
	short f_opthdr;
	/** flags */
	short f_flags;
};

struct ECOFF_MIPS_A_OUT_HDR {
	//public final static int SIZEOF = 56;
	/** magic value - machine specific */
	short magic;
	/** version stamp */
	short vstamp;
	/** text size in bytes */
	long tsize;
	/** initialized data size in bytes */
	long dsize;
	/** uninitialized data size in bytes */
	long bsize;
	/** entry polong */
	long entry;
	/** base of text used for this file */
	long textStart;
	/** base of data used for this file */
	long dataStart;
	/** base of bss used for this file */
	long bssStart;
	/** general purpose register mask */
	long gprmask;
	/** co-processor register mask */
	long cprmask[4];
	/** gp value used for this object */
	long gpValue;
};

struct FDR {
	unsigned long adr;                                                               /* memory address of beginning of file - adr is incorrect if there are no PDR’s for this FDR.*/
	long rss;                                                                        /* file name (of source, if known) */
	long issBase;                                                                    /* Offset with local strings of the beginning of strings for this FDR */
	long cbSs;                                                                       /* number of bytes in the ss */
	long isymBase;                                                                   /* beginning of symbols */
	long csym;                                                                       /* count file’s of symbols */
	long ilineBase;                                                                  /*Index into virtual array of linetable entries(array is size of textfor this fdr).
	 * Array does not existon disk.Array created by libmldby expanding compressed line datavirtual table has a line number(integer) per instruction in text.virtual entry is 32 - bit integer*/
	long cline;                                                                      /* Size of the the virtual array indexed by ilineBase.Count of number of 32 bitwordsilineBase + cline of this FDR are theilineBase of the next FDRthat has a line table(ie, has text).*/
	long ioptBase;                                                                   /* file’s optimization entries */
	long copt;                                                                       /* count of file’s optimization entries */
	unsigned short ipdFirst;                                                         /* start of procedures for this file */
	unsigned short cpd;                                                              /* count of procedures for this file */
	long iauxBase;                                                                   /* file’s auxiliary entries */
	long caux;                                                                       /* count of file’s auxiliary entries */
	long rfdBase;                                                                    /* index into the file indirect table */
	long crfd;                                                                       /* count file indirect entries */
	unsigned lang : 5; /* language for this file */                                  /* language numbers assigned "permanently"*/
	unsigned fMerge : 1; /* whether this file can be merged */                       /* fmerge shows as "mergeable" or "unmergeable"for the file via st_printfd() in stdump*/
	unsigned fReadin : 1; /* true if it was read in (not just created) */            /* Readin shows as "preexisting" if readin set, else" new" via st_printfd() in stdump*/
	unsigned fBigendian : 1; /* if set, was compiled on big endian machine */        /*    auxes will be in compile hosts type */
	unsigned glevel : 2;                                                             /* level this file was compiled with */
	unsigned signedchar : 1; /* whether files was compiled with char being signed */ /* the following were added very late in IRIX 5.2 to allowgreater than 64K procedures */
	unsigned ipdFirstMSBits : 4;                                                     /* upper bits to allow  ipdFirst toexceed 64K entries(These are the most significant bits of what is,after concatenating the bits, a 20 bit number) */
	unsigned cpdMSBits : 4;                                                          /* upper bits to allow cpd to exceed 64Kentries(These are the most significant bits of what is,after concatenating the bits, a 20 bit number) */
	unsigned reserved : 13;                                                          /* reserved for future use */
	long cbLineOffset;                                                               /* byte offset from header for this file ln’s - This is offset of the compressed line table for this fdr (within the physicalcompressed line table).
 * Used by the debugger as a way to start atthe beginning ofinformation for a procedure.
 * The cbLineOffset of the next FDRcbLineOffset canbe a line table stopping rule.(thus, cbLineOffset is non-decreasing).*/
	long cbLine;                                                                     /* size of lines for this file Size in bytes of the compressed line tableof this fdr. cbLIneOffset + cbLine for this fdrare the cbLineOffset of the next FDRthat has a line table (ie, has text).*/
};

struct SYMR {
	long iss;              /* index into String Space of name */
	long value;            /* value of symbol */
	unsigned st : 6;       /* symbol type */
	unsigned sc : 5;       /* storage class - text, data, etc */
	unsigned reserved : 1; /* reserved */
	unsigned index : 20;   /* index into sym/aux table */
};

bool dump_mdebug_section(const std::string& elf_path, const std::string& mdebug_path, std::vector<char>& mdebug_data)
{
	ELFIO::elfio reader;

	if (!reader.load(elf_path.c_str()))
	{
		std::cerr << "Couldn't load ELF at " << elf_path << std::endl;
		return false;
	}

	ELFIO::Elf_Half num_sections = reader.sections.size();
	std::cout << "Number of sections: " << num_sections << std::endl;
	bool mdebug_found = false;

	for (auto& section : reader.sections)
	{
		if (section->get_name() == ".mdebug")
		{
			mdebug_found = true;
		}
	}
	if (!mdebug_found)
	{
		std::cerr << ".mdebug section not found! " << std::endl;
		return false;
	}

	// Now the fun begins
	auto mdebug_section = reader.sections[".mdebug"];
	mdebug_data.assign(mdebug_section->get_data(), mdebug_section->get_data() + mdebug_section->get_size());

	std::ofstream mdebug_raw_file(mdebug_path);
	if (!mdebug_raw_file.is_open())
	{
		std::cerr << "Couldn't open dump file" << std::endl;
		return false;
	}

	mdebug_raw_file.write(mdebug_data.data(), mdebug_data.size());
	mdebug_raw_file.close();

	return true;
}

void process_mdebug(std::vector<char>& mdebug_data)
{
	HDR hdr = *reinterpret_cast<HDR*>(mdebug_data.data());
	ECOFF_MIPS_A_OUT_HDR mips_a_out_hdr = *reinterpret_cast<ECOFF_MIPS_A_OUT_HDR*>(mdebug_data.data() + sizeof(HDR));

	// For each FDR
	// 	Process each Local Symbol
	// 	Process each PDR in this FDR
	// end for
	// For each EXTR
	// 	Process the EXTR
	// 	end for
	size_t size = mdebug_data.size();
	if (hdr.cbFdOffset > size)
	{
		std::cerr << "omg";
	}

	//FDR fdr = *reinterpret_cast<FDR*>(mdebug_data.data() + hdr.cbFdOffset + sizeof(FDR));
	//SYMR sym = *reinterpret_cast<SYMR*>(mdebug_data.data() + hdr.cbSymOffset + fdr.isymBase + sizeof(SYMR));
	//std::cout << sym.index << std::endl;
}

int main(int argc, char* argv[])
{
	std::vector<std::string> args(argv, argv + argc);
	if (args.size() <= 1)
	{
		std::cerr << "Provide an ELF path" << std::endl;
		return 1;
	}
	const std::string& elf_path = args[1];

	// Dump section to file, and return raw byte array for processing
	std::vector<char> mdebug_data;
	if (!dump_mdebug_section(elf_path, ".mdebug-raw", mdebug_data))
	{
		std::cerr << "Error processing ELF" << std::endl;
		return 1;
	}

	// Lets take a looksee at the section
	process_mdebug(mdebug_data);

	// We need to deal with the raw ELF now, to obtain the offset of the A_OUT section of the elf. All HDR offsets are relative to this
	std::ifstream if_raw_elf(elf_path, std::ios_base::binary);
	std::vector<char> raw_elf_data((std::istreambuf_iterator<char>(if_raw_elf)), std::istreambuf_iterator<char>());
	if_raw_elf.close();

	ECOFF_FILE_HDR file_hdr = *reinterpret_cast<ECOFF_FILE_HDR*>(raw_elf_data.data());



	return 0;
}
