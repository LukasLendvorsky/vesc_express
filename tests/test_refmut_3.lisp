
(define a 1)

(define r (setcar a 999))

(and (not r) (= a 1))
