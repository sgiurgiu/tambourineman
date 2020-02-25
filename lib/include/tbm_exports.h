
#ifndef TBM_EXPORT_H
#define TBM_EXPORT_H

#ifdef SHARED_EXPORTS_BUILT_AS_STATIC
#  define TBM_EXPORT
#  define TBM_NO_EXPORT
#else
#  ifndef TBM_EXPORT
#    ifdef tbm_EXPORTS
        /* We are building this library */
#      define TBM_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define TBM_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef TBM_NO_EXPORT
#    define TBM_NO_EXPORT 
#  endif
#endif

#ifndef TBM_DEPRECATED
#  define TBM_DEPRECATED __declspec(deprecated)
#endif

#ifndef TBM_DEPRECATED_EXPORT
#  define TBM_DEPRECATED_EXPORT TBM_EXPORT TBM_DEPRECATED
#endif

#ifndef TBM_DEPRECATED_NO_EXPORT
#  define TBM_DEPRECATED_NO_EXPORT TBM_NO_EXPORT TBM_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef TBM_NO_DEPRECATED
#    define TBM_NO_DEPRECATED
#  endif
#endif

#endif /* TBM_EXPORT_H */
