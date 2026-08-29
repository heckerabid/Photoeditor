int __mingw_argc_storage = 0;
char **__mingw_argv_storage = 0;

int *__imp___argc = &__mingw_argc_storage;
char ***__imp___argv = &__mingw_argv_storage;