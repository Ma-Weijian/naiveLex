# naiveLex
A naive word analyzer which follows the N2176 draft of C17.

It is implemented with object oriented programming style.

It obtains the most naive error correction logic which can be illustrated as follows.

If a faulty identifier 1abc happened at int 1abc, then it will be recognized as a faulty identifier and the suggested modification will be abc.
While if it happened at float trial = temp + 1abc, 1abc will be recognized as a numeric constant and the suggested modification will be 1.
