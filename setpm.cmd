/* setpm.cmd -- Patch OS/2 LX executable to set PM (WINDOWAPI) subsystem type
 * Usage: REXX setpm.cmd <exe-file>
 * Sets e32_mflags bits 9-8 = 11 (0x0300 = WINDOWAPI) in the LX header.
 */
parse arg fname
if fname = '' then do
    say 'Usage: REXX setpm.cmd filename.exe'
    exit 1
end

rc = stream(fname, 'C', 'OPEN BOTH')
if rc \= 'READY:' then do
    say 'Error opening:' fname '(' || rc || ')'
    exit 1
end

/* e_lfanew: 4-byte LE DWORD at MZ offset 0x3C = 1-based position 61 */
raw = charin(fname, 61, 4)
lxoff = c2d(substr(raw,1,1)) + c2d(substr(raw,2,1))*256 +,
        c2d(substr(raw,3,1))*65536 + c2d(substr(raw,4,1))*16777216

/* Verify LX magic */
if charin(fname, lxoff+1, 2) \= 'LX' then do
    say 'Not an LX executable:' fname
    call stream fname, 'C', 'CLOSE'
    exit 1
end

/* e32_mflags: 4-byte LE DWORD at LX offset 0x10 = 1-based file pos lxoff+17 */
flagspos = lxoff + 17
raw = charin(fname, flagspos, 4)
b0 = c2d(substr(raw,1,1))
b1 = c2d(substr(raw,2,1))
b2 = c2d(substr(raw,3,1))
b3 = c2d(substr(raw,4,1))

/* Clear bits 0-2 of byte 1 (= bits 8-10 of mflags) and set WINDOWAPI = 0x300 */
b1 = (b1 % 8) * 8 + 3

call charout fname, d2c(b0), flagspos
call charout fname, d2c(b1)
call charout fname, d2c(b2)
call charout fname, d2c(b3)

call stream fname, 'C', 'CLOSE'
say 'PM (WINDOWAPI 0x0300) type set on' fname
