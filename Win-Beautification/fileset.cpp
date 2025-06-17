#include "fileset.h"

FileSet* FileSet::manager = nullptr;

FileSet* FileSet::instance()
{
    if (!manager)
        manager = new FileSet();

    return manager;
}

FileSet::FileSet() = default;
FileSet::~FileSet() = default;
