#include <iostream>

/*!
 * position an input stream in the start of previous line, before the stream's
 * current position. If num_of_lines > 1, then position the buffer at he start
 * of so many lines.
 * 
 * Exit Codes:
 * 0 -> ok
 * 1 -> reached start of file (!)
 */
int readbackwards (std::istream* is, int num_of_lines)
{

  /* various variables */
  char ch;
  int starts = 0;

  /* pre-set the status */
  int status = 0;

  /* get the first char from the stream */
  is->get (ch);

  /* for as many lines as specified at input */
  while (starts<num_of_lines) {
    //is->get (ch);

    /* warn against EOF bit */
    if (is->eof ()) {
      is->clear ();
      is->seekg (-1,std::ios::cur);
    }

    /* position the stream 1 place to the left */
    while (true) {
      is->seekg (-2,std::ios_base::cur);

      /* caution; this may be the start of the file! */
      if((int)is->tellg()<=1) {
        status = 1;
        return status;

      /* if not, extract the char */
      } else {
        is->get (ch);
      }

      /* and check if it is a newline */
      if (ch=='\n') {
        starts++;
        break;
      }
    }
  }

  /* return the status */
  return status;
}
