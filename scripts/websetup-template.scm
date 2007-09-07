
;========================================================================
;			HTML generation

(require-extension srfi-1)
(require-extension srfi-13)
(require-extension sxml-transforms)
(require-extension doctype)

(define nl (list->string (list #\newline)))

(define lookup-def 
  (lambda (k lst . rest)
    (let-optionals rest ((default #f))
      (alist-ref k lst eq? default))))

(define lookup-field 
  (lambda (k lst . rest)
    (let-optionals rest ((default #f))
      (let loop ((lst lst))
	(if (null? lst) default
	    (let ((elm (car lst)))
	      (match elm
		     ((s . _)  (if (eq? s k) (cdr elm)  (loop (cdr lst))))
		     (else  (loop (cdr lst))))))))))

(define (make-formvar def . rest)
 (let-optionals rest ((parent #f))
   (let ((name  (if parent (string-concatenate (list (symbol->string parent) "_" (symbol->string (car def)))) 
		    (car def)))
	 (dflt  (filter  (lambda (x) (not (list? x))) (cdr def)))
	 (label (lookup-field 'label def))
	 (type  (lookup-field 'type def)))
     `(div 
       ,(if (null? dflt)
	    (let ((elems  (filter (lambda (x) (not (eq? (car x) 'label))) (cdr def))))
	      `(fieldset (@ (class repeat))
		(legend ,(or label (car def)))
		,(map (lambda (x) (list (make-formvar x (car def)) nl)) elems)))
	    `(span (@ (class oneField))
		   ,(if label `(label (@ (for ,name) (class preField)) ,label) (list))
		   ,(match type
			   (('checkbox . _)   
			    `(input (@ (type checkbox) (name ,name) (id ,name) (title ,label) (value ,(car dflt)))))
			   (('textarea rows cols . rest)   
			    `(textarea (@ (name ,name) (id ,name) (rows ,rows) (cols ,cols) (title ,label)) ,nl
				       ,(if (null? rest)
					    (let loop ((lst dflt) (ax (list)))
					      (if (null? lst) (reverse ax)
						  (loop (cdr dflt) (cons nl (cons (car dflt) ax)))))
					    (let ((irows  (car rest))
						  (icols  (cadr rest)))
					      (let ((rows (if (string? irows) (string->number irows) irows))
						    (cols (if (string? icols) (string->number icols) icols)))
						(let rloop ((i rows) (lst dflt) (ax (list)))
						  (let cloop ((j cols) (lst lst) (ax ax))
						    (if (> j 0) (cloop (- j 1) (cdr lst) (cons " " (cons (car lst) ax)))
							(if (> i 1) (rloop (- i 1) lst (cons nl ax))  
							    (reverse (cons nl ax)))))))))))
			   (('select . values)  
			    `(select (@ (name ,name) (id ,name) (title ,label))
				     ,(map (lambda (x) `(option (@ (value ,x) (label ,x)) ,x)) values)))
			   (else  
			    `(input (@ (name ,name) (id ,name) (title ,label) (value ,(car dflt))))))))))))

    

(define (make-header head-parms)
  `(head
    ,nl (title ,(lookup-def 'title head-parms))
    ,nl (meta (@ (http-equiv "Content-Type") (content "text/html; charset=UTF-8")))
    ,nl (meta (@ (http-equiv "Content-Style-Type") (content "text/css")))
    ,nl,(let ((style  (lookup-def 'style head-parms)))
	  (if style `(link (@ (rel "stylesheet") (type "text/css") (href ,style))) '()))
    ,nl ,(zip
	  (map
	   (lambda (key)
	     (let ((val (lookup-def key head-parms )))
	       (and val
		    `(meta (@ (name ,(symbol->string key)) (content ,val))))))
	   '(description AuthorAddress keywords
			 Date-Revision-yyyymmdd Date-Creation-yyyymmdd))
	  (circular-list nl))
    ,nl  ,(let ((links (lookup-def 'Links head-parms '())))
	    (and (pair? links)
		 (map
		  (lambda (link-key)
		    (let ((val (lookup-def link-key links)))
		      (and val
			   (let ((val (if (not (pair? val)) (list val) val)))
			     `(link (@ (rel ,(symbol->string link-key))
				       (href ,(car val))
				       ,@(cdr val)))))))
		  '(start contents prev next))))))


(define (make-footer head-parms)
  `((br)
    (div (hr))
    (h3 "Last updated "
        ,(let* ((date-revised (car (lookup-def 'Date-Revision-yyyymmdd head-parms)))
                (year  (string->number (string-copy date-revised 0 4)))
                (month (string->number (string-copy date-revised 4 6)))
                (day   (string->number (string-copy date-revised 6 8)))
                (month-name
                 (vector-ref
                  '#("January" "February" "March" "April" "May" "June"
                    "July"   "August" "September" "October" "November"
                    "December")
                  (- month 1))))
           (list month-name " " day ", " year))
	,(let ((revision (car (lookup-def 'Revision head-parms))))
	   (if revision (string-concatenate (list " (Revision " revision ") ")) "")))
    ,(let ((links (lookup-def 'Links head-parms '())))
       (and (pair? links)
            (let ((home (lookup-def 'home links)))
              (and home
                   `(p "This site's top page is "
                       (a (@ (href ,home)) (strong ,home)))))))
    (div 
      (address ,(lookup-def 'AuthorAddress head-parms))
       (br)
       "Your comments, problem reports, questions are very welcome!")
    (p (font (@ (size "-2")) "Converted from SXML by SXML->HTML"))
    ,(let ((rcs-id (lookup-def 'rcs-id head-parms)))
       (and rcs-id `(h4 ,rcs-id)))))


(define (generate-HTML Content)
  (let*
      ;; Universal transformation rules. Work for every HTML,
      ;; present and future
   ((universal-conversion-rules
     `((@
	((*default*       ;; local override for attributes
	  . ,(lambda (attr-key . value) (enattr attr-key value))))
	. ,(lambda (trigger . value) (cons '@ value)))
       (*default* . 
	 ,(let ((with-nl	;; Block-level HTML elements:
				;; We insert a NL before them.
				;; No NL is inserted before or after an
				;; inline element.
		  '(br 			;; BR is technically inline, but we
					;; treat it as block
		     p div hr
		     h1 h2 h3 h3 h5 h6
		     dl ul ol li dt dd pre
		     table tr th td
		     center blockquote form
		     address body thead tfoot tbody col colgroup)))
	    (lambda (tag . elems)
	       (let ((nl? (and (memq tag with-nl) #\newline)))
		 (if (and (pair? elems) (pair? (car elems))
		       (eq? '@ (caar elems)))
		   (list nl? #\< tag (cdar elems) #\>
		     (and (pair? (cdr elems))
		       (list (cdr elems) "</" tag #\> nl?)))
		   (list nl? #\< tag #\> 
		     (and (pair? elems) (list elems "</" tag #\> nl?))
		     ))))))
       (*text* . ,(lambda (trigger str) 
		    (if (string? str) (string->goodHTML str) str)))
       (n_		;; a non-breaking space
	. ,(lambda (tag . elems)
	     (list "&nbsp;" elems)))))

    ;; Transformation rules to drop out everything but the
    ;; 'Header' node
    (search-Header-rules
     `((Header *preorder*  . ,(lambda (tag . elems) (cons tag elems)))
       (*default*  . ,(lambda (attr-key . elems)
			(let loop ((elems elems))
			  (cond
			   ((null? elems) '())
			   ((not (pair? (car elems))) (loop (cdr elems)))
			   ((eq? 'Header (caar elems)) (car elems))
			   (else (loop (cdr elems)))))))
       (*text* . ,(lambda (trigger str) '()))))
    )

   (let ((header-parms  (lookup-def 'Header (list (post-order Content search-Header-rules)))))
     (SRV:send-reply
      (pre-post-order Content
       `(
	 ,@universal-conversion-rules
	 
	 (html:begin
	  . ,(lambda (tag . elems)
	       (let ((embedded?    (lookup-def 'Embedded header-parms)))
		 (if embedded? elems
		     (list
		      "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\""
		      nl 
		      "\"http://www.w3.org/TR/html4/loose.dtd\">" nl
		      "<html>" nl
		      elems
		      "</html>" nl)))))

	 (Header
	  *macro*
	  . ,(lambda (tag . headers)
	    (let ((embedded?    (lookup-def 'Embedded header-parms)))
	      (if embedded?  (list)  (make-header headers)))))
	 
	 (body
	  . ,(lambda (tag . elems)
	       (list "<body>" nl elems "</body>")))
	 
	 (page-content *macro*
		       . ,(lambda (tag . elems)
			    `(div (@ (id "content")) ,nl ,elems)))
	 
	 (footer			;; Find the Header in the Content
	  . ,(lambda (tag)		;; and create the footer of the page
	       (post-order (make-footer header-parms)
			   universal-conversion-rules)
	       (list)))
     

	 (page-title *macro*	;; Find the Header in the Content
		     . ,(lambda (tag)		;; and create the page title rule
			  `(div (@ (id "header")) 
				(h1 ,(lookup-def 'title header-parms))
				,nl
				(h2 ,(lookup-def 'subtitle header-parms)))))

	 
	 (abstract			;; The abstract of the document
	  ((Revision
	    . ,(lambda (tag)	;; Find the Header in the Content
		 ;; and create the revision record
		 (list "<strong>Revision "
		       (lookup-def 'Revision header-parms)
		       "</strong>. ")))

	  (keywords . ,(lambda (tag) '()))  )
	 . ,(lambda (tag . abstract-body)
	      (list "<div>" abstract-body "</div>" nl)) )
		 
	 (Section	;; (Section level "content ...")
	  *macro*
	  . ,(lambda (tag level head-word . elems)
	       `((br) (n_) (a (@ (name ,head-word)) (n_))
		 (,(string->symbol (string-append "h" (number->string level)))
		  ,head-word ,elems))))
	 
	 (References	;; (References bibitem ...)
	  *macro*
	  . ,(lambda (tag . bib-items)
	       `((Section 2 "References") ,@bib-items)))
	 
	 
	 (TOC	;; Re-scan the Content for "Section" tags and generate
	  . ,(lambda (tag)	;; the Table of contents
	       (let ((sections
		      (pre-post-order Content
				      `((Section	;; (Section level "content ...")
					 ((*text* . ,(lambda (tag str) str)))
					 . ,(lambda (tag level head-word . elems)
					      (list "<li><a href=\"#" head-word
						    "\">" head-word elems "</a>" nl )))
					(References ;; (References bibitem ...)
					 . ,(lambda (tag . bib-items)
					      (let ((head-word "References"))
						(list "<li><a href=\"#" head-word
						      "\">" head-word "</a>" nl ))))
					(*default*
					 . ,(lambda (tag . elems) elems))
					(*text* . ,(lambda (trigger str) '()))))))
					;(write sections ##stderr)
		 (list "<div id=\"navigation\"><ul>" 
		       "<h3>In this page:</h3>"
		       sections 
		       "</ul></div>" nl))))
	 
	 
	 (verbatim	;; set off pieces of code: one or several lines
	  *macro*
	  . ,(lambda (tag . lines)
	       (cons 'pre
		     (map (lambda (line) (list "     " line nl))
			  lines))))
	 
	 (bibitem
	  *macro*
	  . ,(lambda (tag label key . text)
	       `(p (a (@ (name ,key)) "[" ,label "] ") ,text)))
	 
	 
	 (cite		;; ought to locate the label and use the label!
	  . ,(lambda (tag key)
	       (list "[<a href=\"#" key "\">" key "</a>]")))
	 
	 
	 (url *macro* . ,(lambda (tag href . contents)
			   `(a (@ (href ,href))
			       ,(if (pair? contents)
				    contents
				    href))))
	 (formvar *macro* . 
		  ,(lambda (tag . elems)
		    (list (make-formvar elems) nl)))
	 
	 
	 (sexp		;; S-expression constructor
	  *macro*
	  . ,(lambda (tag . terms)
	       `((code (strong "(") " ") ,(list-intersperse terms " ")
		 (code " " (strong ")")))))
	 
	 (sexp-cons		;; S-expression constructor, like cons
	  *macro*
	  . ,(lambda (tag pcar pcdr)
	       `((code (strong "(") " ") ,pcar (code (strong " . ")) ,pcdr
		 (code " " (strong ")")))))
	 
	 (sset		;; A tagged unordered S-expression (i.e., a set)
	  *macro*
	  . ,(lambda (tag set-tag . terms)
	       `((code (strong "{")) ,(list-intersperse
				       (cons set-tag terms) " ")
		 (code " " (strong "}")))))))))))
  



