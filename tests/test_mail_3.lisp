

(set-mailbox-size 20)

(send (self) 1)
(send (self) 2)
(send (self) 3)
(send (self) 4)
(send (self) 5)
(send (self) 6)
(send (self) 7)
(send (self) 8)
(send (self) 9)
(send (self) 10)
(send (self) 11)
(send (self) 12)
(send (self) 13)
(send (self) 14)
(send (self) 15)
(send (self) 16)
(send (self) 17)
(send (self) 18)
(send (self) 19)
(send (self) 20)


(recv ((? x) (define a1 x)))
(recv ((? x) (define a2 x)))
(recv ((? x) (define a3 x)))
(recv ((? x) (define a4 x)))
(recv ((? x) (define a5 x)))
(recv ((? x) (define a6 x)))
(recv ((? x) (define a7 x)))
(recv ((? x) (define a8 x)))
(recv ((? x) (define a9 x)))
(recv ((? x) (define a10 x)))
(recv ((? x) (define a11 x)))
(recv ((? x) (define a12 x)))
(recv ((? x) (define a13 x)))
(recv ((? x) (define a14 x)))
(recv ((? x) (define a15 x)))
(recv ((? x) (define a16 x)))
(recv ((? x) (define a17 x)))
(recv ((? x) (define a18 x)))
(recv ((? x) (define a19 x)))
(recv ((? x) (define a20 x)))


(and (eq (list a1 a2 a3 a4 a5 a6 a7 a8 a9 a10) (list 1 2 3 4 5 6 7 8 9 10))
     (eq (list a11 a12 a13 a14 a15 a16 a17 a18 a19 a20) (list 11 12 13 14 15 16 17 18 19 20)))
