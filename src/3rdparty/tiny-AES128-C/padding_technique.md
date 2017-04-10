**lsatenstein** commented on 29 May 2016


When encrypting the string, if the string is less than a multiple of 16, pad the string with hex zeros to make the length become an exact multiple of 16. In the final byte of the padded value, indicate as a character (1-x0f), the actual length of the data that is valid within the padded block.

If the string is exactly a multiple of 16 bytes, add a padded block of hex zeros. The final byte will contain hex zero, which states that none of the final block is used.

When decoding, examine the last block of 16 bytes. That final byte indicates how many characters of this last block are valid.

I use this technique elsewhere, When I get a chance I will post my solution. My solution works with ecb or with cypher block chaining mode.


link to issue
https://github.com/kokke/tiny-AES128-C/issues/37#issue-157407453