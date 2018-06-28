#ifndef FILEINFO_H
#define FILEINFO_H

#include <QFlags>

class FileInfo
{
 public:
     enum Option {
         File = 0x0,  // 0x000000
         Comment = 0x1, // 0x000001
         Date = 0x2 // 0x000010
         // ... some more options with value which is a power of two
     };
     Q_DECLARE_FLAGS(Options, Option)
 };
Q_DECLARE_OPERATORS_FOR_FLAGS(FileInfo::Options)

#endif // FILEINFO_H
