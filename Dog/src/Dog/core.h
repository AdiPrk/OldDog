#ifndef DOG_CORE_H
#define DOG_CORE_H

#ifdef DOG_EXPORTS
#define DOG_API __declspec(dllexport)
#else
#define DOG_API __declspec(dllimport)
#endif

#endif // DOG_CORE_H
