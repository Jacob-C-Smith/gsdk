#include <renderer/spectrum.h>

int spectrum_constant_construct ( spectrum *const p_spectrum, const float constant )
{
    
    // Argument check
    if ( p_spectrum == (void *) 0 ) goto no_spectrum;

    // Initialized data
    spectrum _spectrum = { 0 };

    // Populate the spectrum struct
    _spectrum = (spectrum)
    {
        .type       = SPECTRUM_CONSTANT,
        .pfn_lambda = &spectrum_constant_lambda,
        .pfn_max    = &spectrum_constant_lambda,
        .constant   = 
        {
            .constant = constant
        }
    };

    // Return the struct to the caller
    memcpy(p_spectrum, &_spectrum, sizeof(spectrum));

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_spectrum:
                #ifndef NDEBUG
                    log_error("[renderer] [spectrum] Null pointer provided for oaraneter \"p_spectrum\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int spectrum_constant_lambda ( const spectrum *const p_spectrum, float *p_lambda )
{

    // Argument check
    if ( p_spectrum == (void *) 0 ) goto no_spectrum;
    if ( p_lambda   == (void *) 0 ) goto no_lambda;

    // Return the wavelength to the caller
    *p_lambda = p_spectrum->constant.constant;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_spectrum:
                #ifndef NDEBUG
                    log_error("[renderer] [spectrum] Null pointer provided for oaraneter \"p_spectrum\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_lambda:
                #ifndef NDEBUG
                    log_error("[renderer] [spectrum] Null pointer provided for oaraneter \"p_lambda\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int spectrum_densely_sampled_construct ( spectrum *const p_spectrum, const spectrum *const p_source_spectrum, int lambda_min, int lambda_max )
{
    
    // Argument check
    if ( p_spectrum        == (void *) 0 ) goto no_spectrum;
    if ( p_source_spectrum == (void *) 0 ) goto no_source_spectrum;

    // Initialized data
    spectrum _spectrum = { 0 };
    float     _values[RENDERER_SPECTRUM_LAMBDA_MAX - RENDERER_SPECTRUM_LAMBDA_MIN];

    // For each value in the parameterized range ...
    for ( int lambda = lambda_min; lambda <= lambda_max; ++lambda )

        // ... evaluate the wavelength from the source spectrum, and store it
        _values[lambda - lambda_min] = p_source_spectrum->pfn_lambda(p_source_spectrum, lambda);

    // Populate the spectrum struct
    _spectrum = (spectrum)
    {
        .type            = SPECTRUM_CONSTANT,
        .pfn_lambda      = &spectrum_densely_sampled_lambda,
        .pfn_max         = &spectrum_densely_sampled_max,
        .densely_sampled = 
        {
            .lambda_min   = lambda_min,
            .lambda_max   = lambda_max,
            .lambda_range = lambda_max - lambda_min,
            .values       = { 0 }
        }
    };

    // Store the sampled values
    memcpy(&_spectrum.densely_sampled.values, &_values, lambda_max - lambda_min);

    // Return the struct to the caller
    memcpy(p_spectrum, &_spectrum, sizeof(spectrum));

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_spectrum:
                #ifndef NDEBUG
                    log_error("[renderer] [spectrum] Null pointer provided for oaraneter \"p_spectrum\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
            
            no_source_spectrum:
                #ifndef NDEBUG
                    log_error("[renderer] [spectrum] Null pointer provided for oaraneter \"p_source_spectrum\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int spectrum_densely_sampled_lambda ( spectrum *const p_spectrum, float lambda, float *const p_lambda )
{
    
    // Argument check
    if ( p_spectrum == (void *) 0 ) goto no_spectrum;
    if ( p_lambda   == (void *) 0 ) goto no_lambda;

    // Initialized data
    int offset = (int) round(lambda) - RENDERER_SPECTRUM_LAMBDA_MIN;

    // Bounds check
    if ( offset < 0 || offset >= p_spectrum->densely_sampled.lambda_range) 

        // Error
        return 0;

    // Return the wavelength to the caller
    *p_lambda = p_spectrum->densely_sampled.values[offset];

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_spectrum:
                #ifndef NDEBUG
                    log_error("[renderer] [spectrum] Null pointer provided for oaraneter \"p_spectrum\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_lambda:
                #ifndef NDEBUG
                    log_error("[renderer] [spectrum] Null pointer provided for oaraneter \"p_lambda\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}