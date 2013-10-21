//Written by Zev Battad
//GenericFile is the superclass from which all
//specific file-type classes inherit.

#ifndef GENERIC_FILE_H
#define GENERIC_FILE_H

class GenericFile
{
public:
    //Abstract functions
    //TODO: Determine what other common functions are needed for all files
    virtual bool Open();
    virtual bool Close();

};

#endif // GENERIC_FILE_H
