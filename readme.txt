 October 29, 1995

 Oops, we goofed in some of the information we presented concerning
 bitmap colors on pages 14 and 15 of the article.  Some of the
 information was not properly transposed from the sample code, which
 is correct, to the article text.  Yup, too many late night writing
 sessions<G>, as our days are mainly spent coding to keep our ultimate
 employers, our kittys, fit and fed.  Mind you, they were none too
 happy when they found out that we had goofed <VBG>.

 Our apologies for any confusion this may have caused.  The necessary
 revisions are presented below:

 -----------------------------------------------

 The following table depicts the number of colors as defined by cBitCount
 on page 14:

 cBitCount      Colors
 1              2
 4              16
 8              256
 24             16.7-M    (this entry was mistakenly listed as 16 and 65,536)

 -----------------------------------------------

 The paragraph and calculation that immediately follows the
 cBitCount/colors table on page 14 should read as follows:

 The easiest way to determine the number of colours is to take the value of
 1 and rotate it left cBitCount positions:

 cColors = 1 << bfh2.bmp2.cBitCount

 -----------------------------------------------

 Anywhere we mention 64-K color bitmap(s) or 64-K bitmap on pages 14 and
 15 is in error as well.  The 64-K should be 16.7-M (million).

 Mark Benge
 Matt Smith

