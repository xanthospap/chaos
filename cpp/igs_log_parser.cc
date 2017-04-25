#ifndef __IGS_LOGFILE_PARSER__
#define __IGS_LOGFILE_PARSER__

#include <fstream>
#include <cstdlib>
#include <cmath>
#include <stdexcept>
#include <cstring>
#include <map>
#include <iostream>

namespace ngpt
{

class igs_logfile
{
public:
    /// Let's not write this more than once.
    typedef std::ifstream::pos_type pos_type;
    /// A dictionary to hold block numbers and their respective titles.
    static std::map<int, std::string> block_dict;
public:
    /// The constructor will throw, in case the file does not exist, or can't
    /// be opened. In such a case, an std::runtime_error will be thrown.
    /// Once the constructor is done, the file should be open and ready to be
    /// read from.
    /// @param[in] fname The igs log file filename.
    /// @throw     The constructor will throw an std::runtime_error if the
    ///            input filen cannot be opened.
    explicit igs_logfile(const char* fname)
    : __filename{fname},
      __istream {fname, std::ios_base::in},
      __last_block_read{-1}
    {
        if ( !__istream.is_open() )
        {
            throw std::runtime_error
                {"Cannot open log file: " + std::string(fname)};
        }
    }
    /// Destructor (closing the file is not mandatory, but nevertheless)
    ~igs_logfile() noexcept
    { if (__istream.is_open()) __istream.close(); }
    /// Copy not allowed !
    igs_logfile(const igs_logfile&) = delete;
    /// Assignment not allowed !
    igs_logfile& operator=(const igs_logfile&) = delete;
    /// Move Constructor.
    igs_logfile(igs_logfile&& a) noexcept = default;
    /// Move assignment operator.
    igs_logfile& operator=(igs_logfile&& a) = default;
    /// Go to a certain block, identified by its number. The stream buffer is
    /// stoped exactly after the header line is read. E.g., if the input integer
    /// is "1" (corresponding to the block 'Site Identification of the GNSS 
    /// Monument'), the function will return after having read this exact line.
    /// @param[in] block_nr An integer corresponding to the block number we
    ///                     want to read. For more information on block numbers,
    ///                     see the block_dict member variable (map).
    /// @throw     Will throw an std::runtime_error, in case:
    ///            - The block_nr does not correspond to a valid block (in the
    ///              block_dict member variable)
    ///            - The corresponding header string is not found within the
    ///              (igs log) file.
    void
    goto_block_nr(int block_nr)
    {
        // string to hold a line.
        char __line[max_line_chars];

        // if the last block read is greter than the block we must read,
        // go to start of file
        if (__last_block_read >= block_nr) {
            __istream.seekg(0);
        }
        // the title we should read, based on the the block number
        auto bstr = block_dict.find(block_nr);
        if (bstr == block_dict.end()) {
            throw std::runtime_error
                {"Invalid block number: "+std::to_string(block_nr)};
        }
        const char* block_cstr = bstr->second.c_str();
        // start reading file and comparing the block titles
        __istream.getline(__line, max_line_chars);
        while ( __istream && strncmp(__line, block_cstr, max_line_chars) ) {
            __istream.getline(__line, max_line_chars);
        }
        if (!__istream) {
            throw std::runtime_error
                {"Cannot find block nr: "+std::to_string(block_nr)};
        }
        std::cout<<"\nLast line read for block nr "+std::to_string(block_nr)+" is: ";
        std::cout<<"\n\""<<__line<<"\"";
        return;
    }
private:
    std::string   __filename;        ///< The filename (including path)
    std::ifstream __istream;         ///< The infput (file) stream.
    int           __last_block_read; ///< The # of the last block read off from file
    pos_type      __stram_pos;       ///< The current position in file (to assist reading)
    static constexpr int max_line_chars {286}; ///< max chars in line
};

} // end namespace ngpt

#endif

std::map<int, std::string> ngpt::igs_logfile::block_dict = 
{
    { 0, "0.   Form"},
    { 1, "1.   Site Identification of the GNSS Monument"},
    { 2, "2.   Site Location Information"},
    { 3, "3.   GNSS Receiver Information"},
    { 4, "4.   GNSS Antenna Information"},
    { 5, "5.   Surveyed Local Ties"},
    { 6, "6.   Frequency Standard"},
    { 7, "7.   Collocation Information"},
    { 8, "8.   Meteorological Instrumentation"},
    { 9, "9.   Local Ongoing Conditions Possibly Affecting Computed Position"},
    {10, "10.  Local Episodic Effects Possibly Affecting Data Quality"},
    {11, "11.  On-Site, Point of Contact Agency Information"},
    {12, "12.  Responsible Agency (if different from 11.)"},
    {13, "13.  More Information}"}
};

int main()
{
    ngpt::igs_logfile log {"kosg_20160307.log"};
    for (int i=0;i <=13; ++i) log.goto_block_nr(i);
    std::cout<<"\n";
    return 0;
}
