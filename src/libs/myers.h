/*
  The MIT License (MIT)
  Copyright (c) 2014 Martin Sosic
  Permission is hereby granted, free of charge, to any person obtaining a copy of
  this software and associated documentation files (the "Software"), to deal in
  the Software without restriction, including without limitation the rights to
  use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
  the Software, and to permit persons to whom the Software is furnished to do so,
  subject to the following conditions:
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
  FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
  COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
  IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef MYERS_H
#define MYERS_H


#ifdef __cplusplus 
extern "C" {
#endif

// Status codes
#define MYERS_STATUS_OK 0
#define MYERS_STATUS_ERROR 1

// Alignment modes
#define MYERS_MODE_HW  0
#define MYERS_MODE_NW  1
#define MYERS_MODE_SHW 2
#define MYERS_MODE_OV  3

    /**
     * Calculates Levenshtein distance of query and target 
     * using Myers's fast bit-vector algorithm and Ukkonen's algorithm.
     * In Levenshtein distance mismatch and indel have cost of 1, while match has cost of 0.
     * Query and target are represented as arrays of numbers, where each number is 
     *  index of corresponding letter in alphabet. So for example if alphabet is ['A','C','T','G']
     *  and query string is "AACG" and target string is "GATTCGG" then our input query should be
     *  [0,0,1,3] and input target should be [3,0,2,2,1,3,3] (and alphabetLength would be 4).
     * @param [in] query  Array of alphabet indices.
     * @param [in] queryLength
     * @param [in] target  Array of alphabet indices.
     * @param [in] targetLength
     * @param [in] alphabetLength
     * @param [in] k  Non-negative number, constraint for Ukkonen. 
     *                 Only best score <= k will be searched for.
     *                 If k is smaller then calculation is faster.
     *                 If you are interested in score only if it is <= K, set k to K.
     *                 If k is negative then k will be auto-adjusted (increased) until score is found.
     * @param [in] mode  Mode that determines alignment algorithm.
     *                    MYERS_MODE_NW: global (Needleman-Wunsch)
     *                    MYERS_MODE_HW: semi-global. Gaps before and after query are not penalized.
     *                    MYERS_MODE_SHW: semi-global. Gap after query is not penalized.
     *                    MYERS_MODE_OV: semi-global. Gaps before and after query and target are not penalized.
     * @param [in] findAlignment  If true and if score != -1, reconstruction of alignment will be performed
     *                            and alignment will be returned. 
     *                            Notice: Finding aligment will increase execution time
     *                                    and could take large amount of memory.
     * @param [out] score  Best score (smallest edit distance) or -1 if there is no score <= k.
     * @param [out] positions  Array of zero-based positions in target where
     *                         query ends (position of last character) with the best score. 
     *                         If gap after query is penalized, gap counts as part of query (NW),
     *                         otherwise not.
     *                         If there is no score <= k, positions is set to NULL.
     *                         Otherwise, array is returned and it is on you to free it with free().
     * @param [out] numPositions  Number of positions returned.
     * @param [out] alignment  Alignment is found for first position returned.
     *                         Will contain alignment if findAlignment is true and score != -1.
     *                         Otherwise it will be set NULL.
     *                         Alignment is sequence of numbers: 0, 1, 2, 3.
     *                         0 stands for match.
     *                         1 stands for insertion to target.
     *                         2 stands for insertion to query.
     *                         3 stands for mismatch.
     *                         Alignment aligns query to target from begining of query till end of query.
     *                         Alignment ends at @param positions[0] in target.
     *                         If gaps are not penalized, they are not in alignment.
     *                         Needed memory is allocated and given pointer is set to it.
     *                         Important: Do not forget to free memory allocated for alignment!
     *                                    Use free().
     * @param [out] alignmentLength  Length of alignment.
     * @return Status code.
     */
    int myersCalcEditDistance(const unsigned char* query, int queryLength,
                              const unsigned char* target, int targetLength,
                              int alphabetLength, int k, int mode,
                              int* bestScore, int** positions, int* numPositions, 
                              bool findAlignment, unsigned char** alignment,
                              int* alignmentLength, int *ret_k=0);

    /** 
     * Builds cigar string from given alignment sequence.
     * @param [in] alignment  Alignment sequence.
     *                        0 stands for match.
     *                        1 stands for insertion to target.
     *                        2 stands for insertion to query.
     *                        3 stands for mismatch.
     * @param [in] alignmentLength
     * @param [out] cigar  Will contain cigar string.
     *     String is null terminated.
     *     Needed memory is allocated and given pointer is set to it.
     *     Do not forget to free it later using free()!
     * @return Status code.
     */
    int edlibAlignmentToCigar(unsigned char* alignment, int alignmentLength,
                              char** cigar);

#ifdef __cplusplus 
}
#endif

#endif // MYERS_H
