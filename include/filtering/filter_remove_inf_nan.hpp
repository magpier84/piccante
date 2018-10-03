/*

PICCANTE
The hottest HDR imaging library!
http://vcg.isti.cnr.it/piccante

Copyright (C) 2014
Visual Computing Laboratory - ISTI CNR
http://vcg.isti.cnr.it
First author: Francesco Banterle

This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.

*/

#ifndef PIC_FILTERING_FILTER_REMOVE_INF_NAN_HPP
#define PIC_FILTERING_FILTER_REMOVE_INF_NAN_HPP

#include "../filtering/filter.hpp"

namespace pic {

/**
 * @brief The FilterRemoveInfNaN class
 */
class FilterRemoveInfNaN: public Filter
{
protected:

    /**
     * @brief ProcessBBox
     * @param dst
     * @param src
     * @param box
     */
    void ProcessBBox(Image *dst, ImageVec src, BBox *box)
    {
        float values[9];

        int channels = dst->channels;

        for(int j = box->y0; j < box->y1; j++) {
            for(int i = box->x0; i < box->x1; i++) {

                float *tmp_data = (*src[0])(i, j);
                float *tmp_dst  = (*dst   )(i, j);

                for(int ch = 0; ch < channels; ch++) {

                    float val = tmp_data[ch];

                    if(isinf(val) || isnan(val)) {
                        int c2 = 0;

                        for(int k = -1; k <= 1; k++) {
                            for(int l = -1; l <= 1; l++) {

                                float *tmp_val = (*src[0])(i + l, j + k);

                                if(!(isnan(tmp_val[ch]) || isinf(tmp_val[ch]))) {
                                    values[c2] = tmp_val[ch];
                                    c2++;
                                }
                            }
                        }

                        if(c2 == 0) {
                            tmp_dst[ch] = 0.0f;
                        } else {
                            std::sort(values, values + c2);
                            tmp_dst[ch] = values[5];
                        }
                    } else {
                        tmp_dst[ch] = val;
                    }
                }
            }
        }
    }
     

public:
    /**
     * @brief FilterRemoveInfNaN
     */
    FilterRemoveInfNaN()
    {
    }

    /**
     * @brief execute
     * @param imgIn
     * @param imgOut
     * @param threshold_nuked
     * @return
     */
    static Image* execute(Image *imgIn, Image *imgOut)
    {
        FilterRemoveInfNaN filter;
        imgOut = filter.ProcessP(Single(imgIn), imgOut);
        return imgOut;
    }

    /**
     * @brief execute
     * @param nameFileIn
     * @param nameFileOut
     * @return
     */
    static Image* execute(std::string nameFileIn, std::string nameFileOut)
    {
        Image imgIn(nameFileIn);
        Image *imgOut = execute(&imgIn, NULL);
        imgOut->Write(nameFileOut);

        return imgOut;
    }
};

} // end namespace pic

#endif /* PIC_FILTERING_FILTER_REMOVE_INF_NAN_HPP */

