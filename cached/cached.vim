let SessionLoad = 1
if &cp | set nocp | endif
let s:cpo_save=&cpo
set cpo&vim
nmap gx <Plug>NetrwBrowseX
nnoremap <silent> <F7> :TlistToggle
nnoremap <silent> <Plug>NetrwBrowseX :call netrw#NetrwBrowseX(expand("<cWORD>"),0)
nnoremap <silent> <F9> :TlistOpen
nnoremap <silent> <F8> :TlistOpen
let &cpo=s:cpo_save
unlet s:cpo_save
set autoindent
set backspace=indent,eol,start
set errorformat=%-GIn\ file\ included\ %.%#,%*[^\"]\"%f\"%*\\D%l:\ %m,\"%f\"%*\\D%l:\ %m,%-G%f:%l:\ (Each\ undeclared\ identifier\ is\ reported\ only\ once,%-G%f:%l:\ for\ each\ function\ it\ appears\ in.),%-GIn\ file\ included\ from\ %f:%l:%c,%-GIn\ file\ included\ from\ %f:%l,%-Gfrom\ %f:%l:%c,%-Gfrom\ %f:%l,%f:%l:%c:%m,%f(%l):%m,%f:%l:%m,\"%f\"\\,\ line\ %l%*\\D%c%*[^\ ]\ %m,%D%*\\a[%*\\d]:\ Entering\ directory\ `%f',%X%*\\a[%*\\d]:\ Leaving\ directory\ `%f',%D%*\\a:\ Entering\ directory\ `%f',%X%*\\a:\ Leaving\ directory\ `%f',%DMaking\ %*\\a\ in\ %f,%f|%l|\ %m
set fileencodings=ucs-bom,utf-8,default,latin1
set helplang=en
set history=50
set nomodeline
set printoptions=paper:letter
set ruler
set runtimepath=~/.vim,/var/lib/vim/addons,/usr/share/vim/vimfiles,/usr/share/vim/vim73,/usr/share/vim/vimfiles/after,/var/lib/vim/addons/after,~/.vim/after
set shiftwidth=4
set statusline=%<%f%h%m%r%y%=%([%#todo#%{Tlist_Get_Tagname_By_Line()}%*]\ %(%c,%l/%L\ %P%)%)
set suffixes=.bak,~,.swp,.o,.info,.aux,.log,.dvi,.bbl,.blg,.brf,.cb,.ind,.idx,.ilg,.inx,.out,.toc
set switchbuf=useopen
set tabstop=4
set updatetime=2000
let s:so_save = &so | let s:siso_save = &siso | set so=0 siso=0
let v:this_session=expand("<sfile>:p")
silent only
cd ~/git/radarwatch/cached
if expand('%') == '' && !&modified && line('$') <= 1 && getline(1) == ''
  let s:wipebuf = bufnr('%')
endif
set shortmess=aoO
badd +150 cached.cpp
badd +187 cached.h
badd +19 In\ file\ included\ from\ cached.cpp
badd +63 zhelpers.h
args cached.cpp
edit cached.cpp
set splitbelow splitright
wincmd _ | wincmd |
split
1wincmd k
wincmd _ | wincmd |
vsplit
wincmd _ | wincmd |
vsplit
2wincmd h
wincmd w
wincmd w
wincmd _ | wincmd |
split
1wincmd k
wincmd w
wincmd w
set nosplitbelow
set nosplitright
wincmd t
set winheight=1 winwidth=1
exe '1resize ' . ((&lines * 48 + 28) / 57)
exe 'vert 1resize ' . ((&columns * 27 + 106) / 212)
exe '2resize ' . ((&lines * 48 + 28) / 57)
exe 'vert 2resize ' . ((&columns * 95 + 106) / 212)
exe '3resize ' . ((&lines * 24 + 28) / 57)
exe 'vert 3resize ' . ((&columns * 88 + 106) / 212)
exe '4resize ' . ((&lines * 23 + 28) / 57)
exe 'vert 4resize ' . ((&columns * 88 + 106) / 212)
exe '5resize ' . ((&lines * 6 + 28) / 57)
argglobal
enew
file __Tag_List__
let s:cpo_save=&cpo
set cpo&vim
inoremap <buffer> <silent> <kMultiply> :silent! %foldopen!
inoremap <buffer> <silent> <kMinus> :silent! foldclose
inoremap <buffer> <silent> <kPlus> :silent! foldopen
nnoremap <buffer> <silent> * :silent! %foldopen!
nnoremap <buffer> <silent> + :silent! foldopen
nnoremap <buffer> <silent> - :silent! foldclose
nnoremap <buffer> <silent> = :silent! %foldclose
nnoremap <buffer> <silent> q :close
nnoremap <buffer> <silent> <kMultiply> :silent! %foldopen!
nnoremap <buffer> <silent> <kMinus> :silent! foldclose
nnoremap <buffer> <silent> <kPlus> :silent! foldopen
inoremap <buffer> <silent> * :silent! %foldopen!
inoremap <buffer> <silent> + :silent! foldopen
inoremap <buffer> <silent> - :silent! foldclose
inoremap <buffer> <silent> = :silent! %foldclose
inoremap <buffer> <silent> q :close
let &cpo=s:cpo_save
unlet s:cpo_save
setlocal keymap=
setlocal noarabic
setlocal autoindent
setlocal nobinary
setlocal bufhidden=delete
setlocal nobuflisted
setlocal buftype=nofile
setlocal nocindent
setlocal cinkeys=0{,0},0),:,0#,!^F,o,O,e
setlocal cinoptions=
setlocal cinwords=if,else,while,do,for,switch
setlocal colorcolumn=
setlocal comments=s1:/*,mb:*,ex:*/,://,b:#,:%,:XCOMM,n:>,fb:-
setlocal commentstring=/*%s*/
setlocal complete=.,w,b,u,t,i
setlocal concealcursor=
setlocal conceallevel=0
setlocal completefunc=
setlocal nocopyindent
setlocal cryptmethod=
setlocal nocursorbind
setlocal nocursorcolumn
setlocal nocursorline
setlocal define=
setlocal dictionary=
setlocal nodiff
setlocal equalprg=
setlocal errorformat=
setlocal noexpandtab
if &filetype != 'taglist'
setlocal filetype=taglist
endif
setlocal foldcolumn=3
setlocal foldenable
setlocal foldexpr=0
setlocal foldignore=#
setlocal foldlevel=9999
setlocal foldmarker={{{,}}}
setlocal foldmethod=manual
setlocal foldminlines=0
setlocal foldnestmax=20
setlocal foldtext=v:folddashes.getline(v:foldstart)
setlocal formatexpr=
setlocal formatoptions=tcq
setlocal formatlistpat=^\\s*\\d\\+[\\]:.)}\\t\ ]\\s*
setlocal grepprg=
setlocal iminsert=0
setlocal imsearch=0
setlocal include=
setlocal includeexpr=
setlocal indentexpr=
setlocal indentkeys=0{,0},:,0#,!^F,o,O,e
setlocal noinfercase
setlocal iskeyword=@,48-57,_,192-255
setlocal keywordprg=
setlocal nolinebreak
setlocal nolisp
setlocal nolist
setlocal makeprg=
setlocal matchpairs=(:),{:},[:]
setlocal nomodeline
setlocal nomodifiable
setlocal nrformats=octal,hex
set number
setlocal nonumber
setlocal numberwidth=4
setlocal omnifunc=
setlocal path=
setlocal nopreserveindent
setlocal nopreviewwindow
setlocal quoteescape=\\
setlocal noreadonly
setlocal norelativenumber
setlocal norightleft
setlocal rightleftcmd=search
setlocal noscrollbind
setlocal shiftwidth=4
setlocal noshortname
setlocal nosmartindent
setlocal softtabstop=0
setlocal nospell
setlocal spellcapcheck=[.?!]\\_[\\])'\"\	\ ]\\+
setlocal spellfile=
setlocal spelllang=en
setlocal statusline=
setlocal suffixesadd=
setlocal noswapfile
setlocal synmaxcol=3000
if &syntax != 'taglist'
setlocal syntax=taglist
endif
setlocal tabstop=4
setlocal tags=
setlocal textwidth=0
setlocal thesaurus=
setlocal noundofile
setlocal nowinfixheight
set winfixwidth
setlocal winfixwidth
setlocal nowrap
setlocal wrapmargin=0
wincmd w
argglobal
let s:cpo_save=&cpo
set cpo&vim
imap <buffer> <silent> <S-F9> :call C_Arguments()
imap <buffer> <silent> <C-F9> :call C_Run()
imap <buffer> <silent> <F9> :call C_Link():call C_HlMessage()
imap <buffer> <silent> <M-F9> :call C_Compile():call C_HlMessage()
nmap <buffer> <silent> <NL> i=C_JumpCtrlJ()
nnoremap <buffer> <silent> \cl :call C_EndOfLineComment()
nnoremap <buffer> <silent> \c* :call C_CodeToCommentC():nohlsearchj
nnoremap <buffer> <silent> \cc :call C_CodeToCommentCpp():nohlsearchj
nnoremap <buffer> <silent> \co :call C_CommentToCode():nohlsearch
nnoremap <buffer> <silent> \cd :call C_InsertDateAndTime('d')
nnoremap <buffer> <silent> \ct :call C_InsertDateAndTime('dt')
nnoremap <buffer> <silent> \cx :call C_CommentToggle( )
nnoremap <buffer> <silent> \sd :call C_InsertTemplate("statements.do-while")
nnoremap <buffer> <silent> \sfo :call C_InsertTemplate("statements.for-block")
nnoremap <buffer> <silent> \sif :call C_InsertTemplate("statements.if-block")
nnoremap <buffer> <silent> \sie :call C_InsertTemplate("statements.if-else")
nnoremap <buffer> <silent> \sife :call C_InsertTemplate("statements.if-block-else")
nnoremap <buffer> <silent> \se :call C_InsertTemplate("statements.else-block")
nnoremap <buffer> <silent> \swh :call C_InsertTemplate("statements.while-block")
nnoremap <buffer> <silent> \ss :call C_InsertTemplate("statements.switch")
nnoremap <buffer> <silent> \s{ :call C_InsertTemplate("statements.block")
nnoremap <buffer> <silent> \sb :call C_InsertTemplate("statements.block")
nnoremap <buffer> <silent> \pif :call C_InsertTemplate("preprocessor.if-endif")
nnoremap <buffer> <silent> \pie :call C_InsertTemplate("preprocessor.if-else-endif")
nnoremap <buffer> <silent> \pid :call C_InsertTemplate("preprocessor.ifdef-else-endif")
nnoremap <buffer> <silent> \pin :call C_InsertTemplate("preprocessor.ifndef-else-endif")
nnoremap <buffer> <silent> \pind :call C_InsertTemplate("preprocessor.ifndef-def-endif")
nnoremap <buffer> <silent> \pi0 :call C_PPIf0("a")2ji
nnoremap <buffer> <silent> \if :call C_InsertTemplate("idioms.function")
nnoremap <buffer> <silent> \isf :call C_InsertTemplate("idioms.function-static")
nnoremap <buffer> <silent> \im :call C_InsertTemplate("idioms.main")
nnoremap <buffer> <silent> \i0 :call C_CodeFor("up"  )
nnoremap <buffer> <silent> \in :call C_CodeFor("down")
nnoremap <buffer> <silent> \ie :call C_InsertTemplate("idioms.enum")
nnoremap <buffer> <silent> \is :call C_InsertTemplate("idioms.struct")
nnoremap <buffer> <silent> \iu :call C_InsertTemplate("idioms.union")
nnoremap <buffer> <silent> \isi :call C_InsertTemplate("idioms.sizeof")
nnoremap <buffer> <silent> \ias :call C_InsertTemplate("idioms.assert")
nnoremap <buffer> <silent> \ii :call C_InsertTemplate("idioms.open-input-file")
nnoremap <buffer> <silent> \io :call C_InsertTemplate("idioms.open-output-file")
nnoremap <buffer> <silent> \nw :call C_CodeSnippet("w")
nnoremap <buffer> <silent> \np :call C_ProtoPick("function")
nnoremap <buffer> <silent> \nf :call C_ProtoPick("function")
nnoremap <buffer> <silent> \nm :call C_ProtoPick("method")
nnoremap <buffer> <silent> \+tr :call C_InsertTemplate("cpp.try-catch")
nnoremap <buffer> <silent> \+ca :call C_InsertTemplate("cpp.catch")
nnoremap <buffer> <silent> \+c. :call C_InsertTemplate("cpp.catch-points")
nmap <buffer> <silent> \rh :call C_Hardcopy()
vmap <buffer> <silent> \rh :call C_Hardcopy()
vnoremap <buffer> <silent> \+c. :call C_InsertTemplate("cpp.catch-points", "v")
vnoremap <buffer> <silent> \+ca :call C_InsertTemplate("cpp.catch", "v")
vnoremap <buffer> <silent> \+tr :call C_InsertTemplate("cpp.try-catch", "v")
vnoremap <buffer> <silent> \nm :call C_ProtoPick("method")
vnoremap <buffer> <silent> \nf :call C_ProtoPick("function")
vnoremap <buffer> <silent> \np :call C_ProtoPick("function")
vnoremap <buffer> <silent> \nw :call C_CodeSnippet("wv")
vnoremap <buffer> <silent> \io :call C_InsertTemplate("idioms.open-output-file", "v")
vnoremap <buffer> <silent> \ii :call C_InsertTemplate("idioms.open-input-file", "v")
vnoremap <buffer> <silent> \ias :call C_InsertTemplate("idioms.assert", "v")
vnoremap <buffer> <silent> \isi :call C_InsertTemplate("idioms.sizeof", "v")
vnoremap <buffer> <silent> \iu :call C_InsertTemplate("idioms.union" , "v")
vnoremap <buffer> <silent> \is :call C_InsertTemplate("idioms.struct", "v")
vnoremap <buffer> <silent> \ie :call C_InsertTemplate("idioms.enum"  , "v")
vnoremap <buffer> <silent> \in :call C_CodeFor("down")
vnoremap <buffer> <silent> \i0 :call C_CodeFor("up"  )
vnoremap <buffer> <silent> \im :call C_InsertTemplate("idioms.main", "v")
vnoremap <buffer> <silent> \isf :call C_InsertTemplate("idioms.function-static", "v")
vnoremap <buffer> <silent> \if :call C_InsertTemplate("idioms.function", "v")
vnoremap <buffer> <silent> \pi0 :call C_PPIf0("v")
vnoremap <buffer> <silent> \pind :call C_InsertTemplate("preprocessor.ifndef-def-endif", "v")
vnoremap <buffer> <silent> \pin :call C_InsertTemplate("preprocessor.ifndef-else-endif", "v")
vnoremap <buffer> <silent> \pid :call C_InsertTemplate("preprocessor.ifdef-else-endif", "v")
vnoremap <buffer> <silent> \pie :call C_InsertTemplate("preprocessor.if-else-endif", "v")
vnoremap <buffer> <silent> \pif :call C_InsertTemplate("preprocessor.if-endif", "v")
vnoremap <buffer> <silent> \sb :call C_InsertTemplate("statements.block", "v")
vnoremap <buffer> <silent> \s{ :call C_InsertTemplate("statements.block", "v")
vnoremap <buffer> <silent> \ss :call C_InsertTemplate("statements.switch", "v")
vnoremap <buffer> <silent> \swh :call C_InsertTemplate("statements.while-block", "v")
vnoremap <buffer> <silent> \se :call C_InsertTemplate("statements.else-block", "v")
vnoremap <buffer> <silent> \sife :call C_InsertTemplate("statements.if-block-else", "v")
vnoremap <buffer> <silent> \sie :call C_InsertTemplate("statements.if-else", "v")
vnoremap <buffer> <silent> \sif :call C_InsertTemplate("statements.if-block", "v")
vnoremap <buffer> <silent> \sfo :call C_InsertTemplate("statements.for-block", "v")
vnoremap <buffer> <silent> \sd :call C_InsertTemplate("statements.do-while", "v")
vnoremap <buffer> <silent> \cx :call C_CommentToggle( )
vnoremap <buffer> <silent> \ct s:call C_InsertDateAndTime('dt')a
vnoremap <buffer> <silent> \cd s:call C_InsertDateAndTime('d')a
vnoremap <buffer> <silent> \co :call C_CommentToCode():nohlsearch
vnoremap <buffer> <silent> \cc :call C_CodeToCommentCpp():nohlsearchj
vnoremap <buffer> <silent> \c* :call C_CodeToCommentC():nohlsearchj
vnoremap <buffer> <silent> \cl :call C_EndOfLineComment()
onoremap <buffer> <silent> \cl :call C_EndOfLineComment()
onoremap <buffer> <silent> \c* :call C_CodeToCommentC():nohlsearchj
onoremap <buffer> <silent> \cc :call C_CodeToCommentCpp():nohlsearchj
onoremap <buffer> <silent> \co :call C_CommentToCode():nohlsearch
onoremap <buffer> <silent> \cd :call C_InsertDateAndTime('d')
onoremap <buffer> <silent> \ct :call C_InsertDateAndTime('dt')
onoremap <buffer> <silent> \cx :call C_CommentToggle( )
onoremap <buffer> <silent> \sd :call C_InsertTemplate("statements.do-while")
onoremap <buffer> <silent> \sfo :call C_InsertTemplate("statements.for-block")
onoremap <buffer> <silent> \sif :call C_InsertTemplate("statements.if-block")
onoremap <buffer> <silent> \sie :call C_InsertTemplate("statements.if-else")
onoremap <buffer> <silent> \sife :call C_InsertTemplate("statements.if-block-else")
onoremap <buffer> <silent> \se :call C_InsertTemplate("statements.else-block")
onoremap <buffer> <silent> \swh :call C_InsertTemplate("statements.while-block")
onoremap <buffer> <silent> \ss :call C_InsertTemplate("statements.switch")
onoremap <buffer> <silent> \s{ :call C_InsertTemplate("statements.block")
onoremap <buffer> <silent> \sb :call C_InsertTemplate("statements.block")
onoremap <buffer> <silent> \pif :call C_InsertTemplate("preprocessor.if-endif")
onoremap <buffer> <silent> \pie :call C_InsertTemplate("preprocessor.if-else-endif")
onoremap <buffer> <silent> \pid :call C_InsertTemplate("preprocessor.ifdef-else-endif")
onoremap <buffer> <silent> \pin :call C_InsertTemplate("preprocessor.ifndef-else-endif")
onoremap <buffer> <silent> \pind :call C_InsertTemplate("preprocessor.ifndef-def-endif")
onoremap <buffer> <silent> \pi0 :call C_PPIf0("a")2ji
onoremap <buffer> <silent> \if :call C_InsertTemplate("idioms.function")
onoremap <buffer> <silent> \isf :call C_InsertTemplate("idioms.function-static")
onoremap <buffer> <silent> \im :call C_InsertTemplate("idioms.main")
onoremap <buffer> <silent> \i0 :call C_CodeFor("up"  )
onoremap <buffer> <silent> \in :call C_CodeFor("down")
onoremap <buffer> <silent> \ie :call C_InsertTemplate("idioms.enum")
onoremap <buffer> <silent> \is :call C_InsertTemplate("idioms.struct")
onoremap <buffer> <silent> \iu :call C_InsertTemplate("idioms.union")
onoremap <buffer> <silent> \isi :call C_InsertTemplate("idioms.sizeof")
onoremap <buffer> <silent> \ias :call C_InsertTemplate("idioms.assert")
onoremap <buffer> <silent> \ii :call C_InsertTemplate("idioms.open-input-file")
onoremap <buffer> <silent> \io :call C_InsertTemplate("idioms.open-output-file")
onoremap <buffer> <silent> \nw :call C_CodeSnippet("w")
onoremap <buffer> <silent> \np :call C_ProtoPick("function")
onoremap <buffer> <silent> \nf :call C_ProtoPick("function")
onoremap <buffer> <silent> \nm :call C_ProtoPick("method")
onoremap <buffer> <silent> \+tr :call C_InsertTemplate("cpp.try-catch")
onoremap <buffer> <silent> \+ca :call C_InsertTemplate("cpp.catch")
onoremap <buffer> <silent> \+c. :call C_InsertTemplate("cpp.catch-points")
omap <buffer> <silent> \rh :call C_Hardcopy()
map <buffer> <silent> \hm :call C_Help("m")
map <buffer> <silent> \hp :call C_HelpCsupport()
map <buffer> <silent> \ro :call C_Toggle_Gvim_Xterm()
map <buffer> <silent> \rx :call C_XtermSize()
map <buffer> <silent> \rs :call C_Settings()
map <buffer> <silent> \rd :call C_Indent()
map <buffer> <silent> \rpa :call C_SplintArguments()
map <buffer> <silent> \rp :call C_SplintCheck():call C_HlMessage()
map <buffer> <silent> \rma :call C_MakeArguments()
map <buffer> <silent> \rme :call C_ExeToRun()
map <buffer> <silent> \rmc :call C_MakeClean()
map <buffer> <silent> \rcm :call C_ChooseMakefile()
map <buffer> <silent> \rm :call C_Make()
map <buffer> <silent> \ra :call C_Arguments()
map <buffer> <silent> \rr :call C_Run()
map <buffer> <silent> \rl :call C_Link():call C_HlMessage()
map <buffer> <silent> \rc :call C_Compile():call C_HlMessage()
noremap <buffer> <silent> \+ec :call C_InsertTemplate("cpp.error-class")
noremap <buffer> <silent> \+tf :call C_InsertTemplate("cpp.template-function")
noremap <buffer> <silent> \+tai :call C_InsertTemplate("cpp.template-accessor-implementation")
noremap <buffer> <silent> \+tmi :call C_InsertTemplate("cpp.template-method-implementation")
noremap <buffer> <silent> \+tcni :call C_InsertTemplate("cpp.template-class-using-new-implementation")
noremap <buffer> <silent> \+tci :call C_InsertTemplate("cpp.template-class-implementation")
noremap <buffer> <silent> \+tcn :call C_InsertTemplate("cpp.template-class-using-new-definition")
noremap <buffer> <silent> \+tc :call C_InsertTemplate("cpp.template-class-definition")
noremap <buffer> <silent> \+ai :call C_InsertTemplate("cpp.accessor-implementation")
noremap <buffer> <silent> \+mi :call C_InsertTemplate("cpp.method-implementation")
noremap <buffer> <silent> \+cni :call C_InsertTemplate("cpp.class-using-new-implementation")
noremap <buffer> <silent> \+ci :call C_InsertTemplate("cpp.class-implementation")
noremap <buffer> <silent> \+cn :call C_InsertTemplate("cpp.class-using-new-definition")
noremap <buffer> <silent> \+c :call C_InsertTemplate("cpp.class-definition")
noremap <buffer> <silent> \+co :call C_InsertTemplate("cpp.cout")
noremap <buffer> <silent> \+" :call C_InsertTemplate("cpp.cout-operator")
noremap <buffer> \nts :CStyle 
noremap <buffer> <silent> \ntr :call C_RereadTemplates()
noremap <buffer> <silent> \ntl :call C_BrowseTemplateFiles("Local")
noremap <buffer> <silent> \ns :call C_ProtoShow()
noremap <buffer> <silent> \nc :call C_ProtoClear()
noremap <buffer> <silent> \ni :call C_ProtoInsert()
noremap <buffer> <silent> \ne :call C_CodeSnippet("e")
noremap <buffer> <silent> \nr :call C_CodeSnippet("r")
noremap <buffer> <silent> \ifp :call C_InsertTemplate("idioms.fprintf")
noremap <buffer> <silent> \ifs :call C_InsertTemplate("idioms.fscanf")
noremap <buffer> <silent> \ire :call C_InsertTemplate("idioms.realloc")
noremap <buffer> <silent> \ima :call C_InsertTemplate("idioms.malloc")
noremap <buffer> <silent> \ica :call C_InsertTemplate("idioms.calloc")
noremap <buffer> <silent> \isc :call C_InsertTemplate("idioms.scanf")
noremap <buffer> <silent> \ip :call C_InsertTemplate("idioms.printf")
noremap <buffer> <silent> \pp :call C_InsertTemplate("preprocessor.pragma")
noremap <buffer> <silent> \pl :call C_InsertTemplate("preprocessor.line")
noremap <buffer> <silent> \pe :call C_InsertTemplate("preprocessor.error")
noremap <buffer> <silent> \pr0 :call C_PPIf0Remove()
noremap <buffer> <silent> \pu :call C_InsertTemplate("preprocessor.undefine")
noremap <buffer> <silent> \pd :call C_InsertTemplate("preprocessor.define")
noremap <buffer> <silent> \p" :call C_InsertTemplate("preprocessor.include-local")
noremap <buffer> <silent> \p< :call C_InsertTemplate("preprocessor.include-global")
noremap <buffer> \+pc :IncludeCppCLibrary 
noremap <buffer> \+ps :IncludeCppLibrary 
noremap <buffer> \pc :IncludeC99Library 
noremap <buffer> \ps :IncludeStdLibrary 
noremap <buffer> <silent> \sc :call C_InsertTemplate("statements.case")
noremap <buffer> <silent> \sw :call C_InsertTemplate("statements.while")
noremap <buffer> <silent> \si :call C_InsertTemplate("statements.if")
noremap <buffer> <silent> \sf :call C_InsertTemplate("statements.for")
noremap <buffer> \csc :SpecialComment 
noremap <buffer> \ckc :KeywordComment 
noremap <buffer> \chs :HFileSection 
noremap <buffer> \ccs :CFileSection 
noremap <buffer> <silent> \cfdh :call C_InsertTemplate("comment.file-description-header")
noremap <buffer> <silent> \cfdi :call C_InsertTemplate("comment.file-description")
noremap <buffer> <silent> \ccl :call C_InsertTemplate("comment.class")
noremap <buffer> <silent> \cme :call C_InsertTemplate("comment.method")
noremap <buffer> <silent> \cfu :call C_InsertTemplate("comment.function")
noremap <buffer> <silent> \cfr :call C_InsertTemplate("comment.frame")
noremap <buffer> <silent> \cs :call C_GetLineEndCommCol()
vnoremap <buffer> <silent> \cj :call C_AdjustLineEndComm()
nnoremap <buffer> <silent> \cj :call C_AdjustLineEndComm()
map <buffer> <silent> <S-F9> :call C_Arguments()
map <buffer> <silent> <C-F9> :call C_Run()
map <buffer> <silent> <F9> :call C_Link():call C_HlMessage()
map <buffer> <silent> <M-F9> :call C_Compile():call C_HlMessage()
imap <buffer> <silent> <NL> =C_JumpCtrlJ()
imap <buffer> <silent> \hm :call C_Help("m")
imap <buffer> <silent> \hp :call C_HelpCsupport()
imap <buffer> <silent> \ro :call C_Toggle_Gvim_Xterm()
imap <buffer> <silent> \rx :call C_XtermSize()
imap <buffer> <silent> \rs :call C_Settings()
imap <buffer> <silent> \rh :call C_Hardcopy()
imap <buffer> <silent> \rd :call C_Indent()
imap <buffer> <silent> \rpa :call C_SplintArguments()
imap <buffer> <silent> \rp :call C_SplintCheck():call C_HlMessage()
imap <buffer> <silent> \rma :call C_MakeArguments()
imap <buffer> <silent> \rme :call C_ExeToRun()
imap <buffer> <silent> \rmc :call C_MakeClean()
imap <buffer> <silent> \rm :call C_Make()
imap <buffer> <silent> \ra :call C_Arguments()
imap <buffer> <silent> \rr :call C_Run()
imap <buffer> <silent> \rl :call C_Link():call C_HlMessage()
imap <buffer> <silent> \rc :call C_Compile():call C_HlMessage()
inoremap <buffer> <silent> \+c. :call C_InsertTemplate("cpp.catch-points")
inoremap <buffer> <silent> \+ca :call C_InsertTemplate("cpp.catch")
inoremap <buffer> <silent> \+tr :call C_InsertTemplate("cpp.try-catch")
inoremap <buffer> <silent> \+ec :call C_InsertTemplate("cpp.error-class")
inoremap <buffer> <silent> \+tf :call C_InsertTemplate("cpp.template-function")
inoremap <buffer> <silent> \+tai :call C_InsertTemplate("cpp.template-accessor-implementation")
inoremap <buffer> <silent> \+tmi :call C_InsertTemplate("cpp.template-method-implementation")
inoremap <buffer> <silent> \+tcni :call C_InsertTemplate("cpp.template-class-using-new-implementation")
inoremap <buffer> <silent> \+tci :call C_InsertTemplate("cpp.template-class-implementation")
inoremap <buffer> <silent> \+tcn :call C_InsertTemplate("cpp.template-class-using-new-definition")
inoremap <buffer> <silent> \+tc :call C_InsertTemplate("cpp.template-class-definition")
inoremap <buffer> <silent> \+ai :call C_InsertTemplate("cpp.accessor-implementation")
inoremap <buffer> <silent> \+mi :call C_InsertTemplate("cpp.method-implementation")
inoremap <buffer> <silent> \+cni :call C_InsertTemplate("cpp.class-using-new-implementation")
inoremap <buffer> <silent> \+ci :call C_InsertTemplate("cpp.class-implementation")
inoremap <buffer> <silent> \+cn :call C_InsertTemplate("cpp.class-using-new-definition")
inoremap <buffer> <silent> \+c :call C_InsertTemplate("cpp.class-definition")
inoremap <buffer> <silent> \+co :call C_InsertTemplate("cpp.cout")
inoremap <buffer> <silent> \+" :call C_InsertTemplate("cpp.cout-operator")
inoremap <buffer> \nts :CStyle 
inoremap <buffer> <silent> \ntr :call C_RereadTemplates()
inoremap <buffer> <silent> \ntl :call C_BrowseTemplateFiles("Local")
inoremap <buffer> <silent> \ns :call C_ProtoShow()
inoremap <buffer> <silent> \nc :call C_ProtoClear()
inoremap <buffer> <silent> \ni :call C_ProtoInsert()
inoremap <buffer> <silent> \nm :call C_ProtoPick("method")
inoremap <buffer> <silent> \nf :call C_ProtoPick("function")
inoremap <buffer> <silent> \np :call C_ProtoPick("function")
inoremap <buffer> <silent> \ne :call C_CodeSnippet("e")
inoremap <buffer> <silent> \nw :call C_CodeSnippet("w")
inoremap <buffer> <silent> \nr :call C_CodeSnippet("r")
inoremap <buffer> <silent> \ifp :call C_InsertTemplate("idioms.fprintf")
inoremap <buffer> <silent> \ifs :call C_InsertTemplate("idioms.fscanf")
inoremap <buffer> <silent> \io :call C_InsertTemplate("idioms.open-output-file")
inoremap <buffer> <silent> \ii :call C_InsertTemplate("idioms.open-input-file")
inoremap <buffer> <silent> \ias :call C_InsertTemplate("idioms.assert")
inoremap <buffer> <silent> \isi :call C_InsertTemplate("idioms.sizeof")
inoremap <buffer> <silent> \ire :call C_InsertTemplate("idioms.realloc")
inoremap <buffer> <silent> \ima :call C_InsertTemplate("idioms.malloc")
inoremap <buffer> <silent> \ica :call C_InsertTemplate("idioms.calloc")
inoremap <buffer> <silent> \isc :call C_InsertTemplate("idioms.scanf")
inoremap <buffer> <silent> \ip :call C_InsertTemplate("idioms.printf")
inoremap <buffer> <silent> \iu :call C_InsertTemplate("idioms.union")
inoremap <buffer> <silent> \is :call C_InsertTemplate("idioms.struct")
inoremap <buffer> <silent> \ie :call C_InsertTemplate("idioms.enum")
inoremap <buffer> <silent> \in :call C_CodeFor("down")i
inoremap <buffer> <silent> \i0 :call C_CodeFor("up"  )i
inoremap <buffer> <silent> \im :call C_InsertTemplate("idioms.main")
inoremap <buffer> <silent> \isf :call C_InsertTemplate("idioms.function-static")
inoremap <buffer> <silent> \if :call C_InsertTemplate("idioms.function")
inoremap <buffer> <silent> \pp :call C_InsertTemplate("preprocessor.pragma")
inoremap <buffer> <silent> \pl :call C_InsertTemplate("preprocessor.line")
inoremap <buffer> <silent> \pe :call C_InsertTemplate("preprocessor.error")
inoremap <buffer> <silent> \pr0 :call C_PPIf0Remove()
inoremap <buffer> <silent> \pi0 :call C_PPIf0("a")2ji
inoremap <buffer> <silent> \pind :call C_InsertTemplate("preprocessor.ifndef-def-endif")
inoremap <buffer> <silent> \pin :call C_InsertTemplate("preprocessor.ifndef-else-endif")
inoremap <buffer> <silent> \pid :call C_InsertTemplate("preprocessor.ifdef-else-endif")
inoremap <buffer> <silent> \pie :call C_InsertTemplate("preprocessor.if-else-endif")
inoremap <buffer> <silent> \pif :call C_InsertTemplate("preprocessor.if-endif")
inoremap <buffer> <silent> \pu :call C_InsertTemplate("preprocessor.undefine")
inoremap <buffer> <silent> \pd :call C_InsertTemplate("preprocessor.define")
inoremap <buffer> <silent> \p" :call C_InsertTemplate("preprocessor.include-local")
inoremap <buffer> <silent> \p< :call C_InsertTemplate("preprocessor.include-global")
inoremap <buffer> \+pc :IncludeCppC9Library 
inoremap <buffer> \+ps :IncludeCppLibrary 
inoremap <buffer> \pc :IncludeC99Library 
inoremap <buffer> \ps :IncludeStdLibrary 
inoremap <buffer> <silent> \sb :call C_InsertTemplate("statements.block")
inoremap <buffer> <silent> \s{ :call C_InsertTemplate("statements.block")
inoremap <buffer> <silent> \sc :call C_InsertTemplate("statements.case")
inoremap <buffer> <silent> \ss :call C_InsertTemplate("statements.switch")
inoremap <buffer> <silent> \swh :call C_InsertTemplate("statements.while-block")
inoremap <buffer> <silent> \sw :call C_InsertTemplate("statements.while")
inoremap <buffer> <silent> \se :call C_InsertTemplate("statements.else-block")
inoremap <buffer> <silent> \sife :call C_InsertTemplate("statements.if-block-else")
inoremap <buffer> <silent> \sie :call C_InsertTemplate("statements.if-else")
inoremap <buffer> <silent> \sif :call C_InsertTemplate("statements.if-block")
inoremap <buffer> <silent> \si :call C_InsertTemplate("statements.if")
inoremap <buffer> <silent> \sfo :call C_InsertTemplate("statements.for-block")
inoremap <buffer> <silent> \sf :call C_InsertTemplate("statements.for")
inoremap <buffer> <silent> \sd :call C_InsertTemplate("statements.do-while")
inoremap <buffer> \csc :SpecialComment 
inoremap <buffer> \ckc :KeywordComment 
inoremap <buffer> \chs :HFileSection 
inoremap <buffer> \ccs :CFileSection 
inoremap <buffer> <silent> \cx :call C_CommentToggle( )
inoremap <buffer> <silent> \ct :call C_InsertDateAndTime('dt')a
inoremap <buffer> <silent> \cd :call C_InsertDateAndTime('d')a
inoremap <buffer> <silent> \cfdh :call C_InsertTemplate("comment.file-description-header")
inoremap <buffer> <silent> \cfdi :call C_InsertTemplate("comment.file-description")
inoremap <buffer> <silent> \ccl :call C_InsertTemplate("comment.class")
inoremap <buffer> <silent> \cme :call C_InsertTemplate("comment.method")
inoremap <buffer> <silent> \cfu :call C_InsertTemplate("comment.function")
inoremap <buffer> <silent> \cfr :call C_InsertTemplate("comment.frame")
inoremap <buffer> <silent> \cj :call C_AdjustLineEndComm()a
inoremap <buffer> <silent> \cl :call C_EndOfLineComment()
let &cpo=s:cpo_save
unlet s:cpo_save
setlocal keymap=
setlocal noarabic
setlocal autoindent
setlocal nobinary
setlocal bufhidden=
setlocal buflisted
setlocal buftype=
setlocal nocindent
setlocal cinkeys=0{,0},0),:,0#,!^F,o,O,e
setlocal cinoptions=
setlocal cinwords=if,else,while,do,for,switch
setlocal colorcolumn=
setlocal comments=s1:/*,mb:*,ex:*/,://,b:#,:%,:XCOMM,n:>,fb:-
setlocal commentstring=/*%s*/
setlocal complete=.,w,b,u,t,i
setlocal concealcursor=
setlocal conceallevel=0
setlocal completefunc=
setlocal nocopyindent
setlocal cryptmethod=
setlocal nocursorbind
setlocal nocursorcolumn
setlocal nocursorline
setlocal define=
setlocal dictionary=~/.vim/c-support/wordlists/c-c++-keywords.list,~/.vim/c-support/wordlists/k+r.list,~/.vim/c-support/wordlists/stl_index.list
setlocal nodiff
setlocal equalprg=
setlocal errorformat=
setlocal noexpandtab
if &filetype != 'cpp'
setlocal filetype=cpp
endif
setlocal foldcolumn=0
setlocal foldenable
setlocal foldexpr=0
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldmarker={{{,}}}
setlocal foldmethod=manual
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldtext=foldtext()
setlocal formatexpr=
setlocal formatoptions=tcq
setlocal formatlistpat=^\\s*\\d\\+[\\]:.)}\\t\ ]\\s*
setlocal grepprg=
setlocal iminsert=0
setlocal imsearch=0
setlocal include=
setlocal includeexpr=
setlocal indentexpr=
setlocal indentkeys=0{,0},:,0#,!^F,o,O,e
setlocal noinfercase
setlocal iskeyword=@,48-57,_,192-255
setlocal keywordprg=
setlocal nolinebreak
setlocal nolisp
setlocal nolist
setlocal makeprg=
setlocal matchpairs=(:),{:},[:]
setlocal nomodeline
setlocal modifiable
setlocal nrformats=octal,hex
set number
setlocal number
setlocal numberwidth=4
setlocal omnifunc=
setlocal path=
setlocal nopreserveindent
setlocal nopreviewwindow
setlocal quoteescape=\\
setlocal noreadonly
setlocal norelativenumber
setlocal norightleft
setlocal rightleftcmd=search
setlocal noscrollbind
setlocal shiftwidth=4
setlocal noshortname
setlocal nosmartindent
setlocal softtabstop=0
setlocal nospell
setlocal spellcapcheck=[.?!]\\_[\\])'\"\	\ ]\\+
setlocal spellfile=
setlocal spelllang=en
setlocal statusline=
setlocal suffixesadd=
setlocal swapfile
setlocal synmaxcol=3000
if &syntax != 'cpp'
setlocal syntax=cpp
endif
setlocal tabstop=4
setlocal tags=
setlocal textwidth=0
setlocal thesaurus=
setlocal noundofile
setlocal nowinfixheight
setlocal nowinfixwidth
setlocal wrap
setlocal wrapmargin=0
silent! normal! zE
let s:l = 2320 - ((26 * winheight(0) + 24) / 48)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
2320
normal! 01l
wincmd w
argglobal
edit cached.h
let s:cpo_save=&cpo
set cpo&vim
imap <buffer> <silent> <S-F9> :call C_Arguments()
imap <buffer> <silent> <C-F9> :call C_Run()
imap <buffer> <silent> <F9> :call C_Link():call C_HlMessage()
imap <buffer> <silent> <M-F9> :call C_Compile():call C_HlMessage()
nmap <buffer> <silent> <NL> i=C_JumpCtrlJ()
nnoremap <buffer> <silent> \cl :call C_EndOfLineComment()
nnoremap <buffer> <silent> \c* :call C_CodeToCommentC():nohlsearchj
nnoremap <buffer> <silent> \cc :call C_CodeToCommentCpp():nohlsearchj
nnoremap <buffer> <silent> \co :call C_CommentToCode():nohlsearch
nnoremap <buffer> <silent> \cd :call C_InsertDateAndTime('d')
nnoremap <buffer> <silent> \ct :call C_InsertDateAndTime('dt')
nnoremap <buffer> <silent> \cx :call C_CommentToggle( )
nnoremap <buffer> <silent> \sd :call C_InsertTemplate("statements.do-while")
nnoremap <buffer> <silent> \sfo :call C_InsertTemplate("statements.for-block")
nnoremap <buffer> <silent> \sif :call C_InsertTemplate("statements.if-block")
nnoremap <buffer> <silent> \sie :call C_InsertTemplate("statements.if-else")
nnoremap <buffer> <silent> \sife :call C_InsertTemplate("statements.if-block-else")
nnoremap <buffer> <silent> \se :call C_InsertTemplate("statements.else-block")
nnoremap <buffer> <silent> \swh :call C_InsertTemplate("statements.while-block")
nnoremap <buffer> <silent> \ss :call C_InsertTemplate("statements.switch")
nnoremap <buffer> <silent> \s{ :call C_InsertTemplate("statements.block")
nnoremap <buffer> <silent> \sb :call C_InsertTemplate("statements.block")
nnoremap <buffer> <silent> \pif :call C_InsertTemplate("preprocessor.if-endif")
nnoremap <buffer> <silent> \pie :call C_InsertTemplate("preprocessor.if-else-endif")
nnoremap <buffer> <silent> \pid :call C_InsertTemplate("preprocessor.ifdef-else-endif")
nnoremap <buffer> <silent> \pin :call C_InsertTemplate("preprocessor.ifndef-else-endif")
nnoremap <buffer> <silent> \pind :call C_InsertTemplate("preprocessor.ifndef-def-endif")
nnoremap <buffer> <silent> \pi0 :call C_PPIf0("a")2ji
nnoremap <buffer> <silent> \if :call C_InsertTemplate("idioms.function")
nnoremap <buffer> <silent> \isf :call C_InsertTemplate("idioms.function-static")
nnoremap <buffer> <silent> \im :call C_InsertTemplate("idioms.main")
nnoremap <buffer> <silent> \i0 :call C_CodeFor("up"  )
nnoremap <buffer> <silent> \in :call C_CodeFor("down")
nnoremap <buffer> <silent> \ie :call C_InsertTemplate("idioms.enum")
nnoremap <buffer> <silent> \is :call C_InsertTemplate("idioms.struct")
nnoremap <buffer> <silent> \iu :call C_InsertTemplate("idioms.union")
nnoremap <buffer> <silent> \isi :call C_InsertTemplate("idioms.sizeof")
nnoremap <buffer> <silent> \ias :call C_InsertTemplate("idioms.assert")
nnoremap <buffer> <silent> \ii :call C_InsertTemplate("idioms.open-input-file")
nnoremap <buffer> <silent> \io :call C_InsertTemplate("idioms.open-output-file")
nnoremap <buffer> <silent> \nw :call C_CodeSnippet("w")
nnoremap <buffer> <silent> \np :call C_ProtoPick("function")
nnoremap <buffer> <silent> \nf :call C_ProtoPick("function")
nnoremap <buffer> <silent> \nm :call C_ProtoPick("method")
nnoremap <buffer> <silent> \+tr :call C_InsertTemplate("cpp.try-catch")
nnoremap <buffer> <silent> \+ca :call C_InsertTemplate("cpp.catch")
nnoremap <buffer> <silent> \+c. :call C_InsertTemplate("cpp.catch-points")
nmap <buffer> <silent> \rh :call C_Hardcopy()
map <buffer> <silent> \hm :call C_Help("m")
map <buffer> <silent> \hp :call C_HelpCsupport()
map <buffer> <silent> \ro :call C_Toggle_Gvim_Xterm()
map <buffer> <silent> \rx :call C_XtermSize()
vmap <buffer> <silent> \rh :call C_Hardcopy()
map <buffer> <silent> \rs :call C_Settings()
omap <buffer> <silent> \rh :call C_Hardcopy()
map <buffer> <silent> \rd :call C_Indent()
map <buffer> <silent> \rpa :call C_SplintArguments()
map <buffer> <silent> \rp :call C_SplintCheck():call C_HlMessage()
map <buffer> <silent> \rma :call C_MakeArguments()
map <buffer> <silent> \rme :call C_ExeToRun()
map <buffer> <silent> \rmc :call C_MakeClean()
map <buffer> <silent> \rcm :call C_ChooseMakefile()
map <buffer> <silent> \rm :call C_Make()
map <buffer> <silent> \ra :call C_Arguments()
map <buffer> <silent> \rr :call C_Run()
map <buffer> <silent> \rl :call C_Link():call C_HlMessage()
map <buffer> <silent> \rc :call C_Compile():call C_HlMessage()
vnoremap <buffer> <silent> \+c. :call C_InsertTemplate("cpp.catch-points", "v")
onoremap <buffer> <silent> \+c. :call C_InsertTemplate("cpp.catch-points")
vnoremap <buffer> <silent> \+ca :call C_InsertTemplate("cpp.catch", "v")
onoremap <buffer> <silent> \+ca :call C_InsertTemplate("cpp.catch")
vnoremap <buffer> <silent> \+tr :call C_InsertTemplate("cpp.try-catch", "v")
onoremap <buffer> <silent> \+tr :call C_InsertTemplate("cpp.try-catch")
noremap <buffer> <silent> \+ec :call C_InsertTemplate("cpp.error-class")
noremap <buffer> <silent> \+tf :call C_InsertTemplate("cpp.template-function")
noremap <buffer> <silent> \+tai :call C_InsertTemplate("cpp.template-accessor-implementation")
noremap <buffer> <silent> \+tmi :call C_InsertTemplate("cpp.template-method-implementation")
noremap <buffer> <silent> \+tcni :call C_InsertTemplate("cpp.template-class-using-new-implementation")
noremap <buffer> <silent> \+tci :call C_InsertTemplate("cpp.template-class-implementation")
noremap <buffer> <silent> \+tcn :call C_InsertTemplate("cpp.template-class-using-new-definition")
noremap <buffer> <silent> \+tc :call C_InsertTemplate("cpp.template-class-definition")
noremap <buffer> <silent> \+ai :call C_InsertTemplate("cpp.accessor-implementation")
noremap <buffer> <silent> \+mi :call C_InsertTemplate("cpp.method-implementation")
noremap <buffer> <silent> \+cni :call C_InsertTemplate("cpp.class-using-new-implementation")
noremap <buffer> <silent> \+ci :call C_InsertTemplate("cpp.class-implementation")
noremap <buffer> <silent> \+cn :call C_InsertTemplate("cpp.class-using-new-definition")
noremap <buffer> <silent> \+c :call C_InsertTemplate("cpp.class-definition")
noremap <buffer> <silent> \+co :call C_InsertTemplate("cpp.cout")
noremap <buffer> <silent> \+" :call C_InsertTemplate("cpp.cout-operator")
noremap <buffer> \nts :CStyle 
noremap <buffer> <silent> \ntr :call C_RereadTemplates()
noremap <buffer> <silent> \ntl :call C_BrowseTemplateFiles("Local")
noremap <buffer> <silent> \ns :call C_ProtoShow()
noremap <buffer> <silent> \nc :call C_ProtoClear()
noremap <buffer> <silent> \ni :call C_ProtoInsert()
vnoremap <buffer> <silent> \nm :call C_ProtoPick("method")
onoremap <buffer> <silent> \nm :call C_ProtoPick("method")
vnoremap <buffer> <silent> \nf :call C_ProtoPick("function")
onoremap <buffer> <silent> \nf :call C_ProtoPick("function")
vnoremap <buffer> <silent> \np :call C_ProtoPick("function")
onoremap <buffer> <silent> \np :call C_ProtoPick("function")
noremap <buffer> <silent> \ne :call C_CodeSnippet("e")
vnoremap <buffer> <silent> \nw :call C_CodeSnippet("wv")
onoremap <buffer> <silent> \nw :call C_CodeSnippet("w")
noremap <buffer> <silent> \nr :call C_CodeSnippet("r")
noremap <buffer> <silent> \ifp :call C_InsertTemplate("idioms.fprintf")
noremap <buffer> <silent> \ifs :call C_InsertTemplate("idioms.fscanf")
vnoremap <buffer> <silent> \io :call C_InsertTemplate("idioms.open-output-file", "v")
onoremap <buffer> <silent> \io :call C_InsertTemplate("idioms.open-output-file")
vnoremap <buffer> <silent> \ii :call C_InsertTemplate("idioms.open-input-file", "v")
onoremap <buffer> <silent> \ii :call C_InsertTemplate("idioms.open-input-file")
vnoremap <buffer> <silent> \ias :call C_InsertTemplate("idioms.assert", "v")
onoremap <buffer> <silent> \ias :call C_InsertTemplate("idioms.assert")
vnoremap <buffer> <silent> \isi :call C_InsertTemplate("idioms.sizeof", "v")
onoremap <buffer> <silent> \isi :call C_InsertTemplate("idioms.sizeof")
noremap <buffer> <silent> \ire :call C_InsertTemplate("idioms.realloc")
noremap <buffer> <silent> \ima :call C_InsertTemplate("idioms.malloc")
noremap <buffer> <silent> \ica :call C_InsertTemplate("idioms.calloc")
noremap <buffer> <silent> \isc :call C_InsertTemplate("idioms.scanf")
noremap <buffer> <silent> \ip :call C_InsertTemplate("idioms.printf")
vnoremap <buffer> <silent> \iu :call C_InsertTemplate("idioms.union" , "v")
onoremap <buffer> <silent> \iu :call C_InsertTemplate("idioms.union")
vnoremap <buffer> <silent> \is :call C_InsertTemplate("idioms.struct", "v")
onoremap <buffer> <silent> \is :call C_InsertTemplate("idioms.struct")
vnoremap <buffer> <silent> \ie :call C_InsertTemplate("idioms.enum"  , "v")
onoremap <buffer> <silent> \ie :call C_InsertTemplate("idioms.enum")
vnoremap <buffer> <silent> \in :call C_CodeFor("down")
onoremap <buffer> <silent> \in :call C_CodeFor("down")
vnoremap <buffer> <silent> \i0 :call C_CodeFor("up"  )
onoremap <buffer> <silent> \i0 :call C_CodeFor("up"  )
vnoremap <buffer> <silent> \im :call C_InsertTemplate("idioms.main", "v")
onoremap <buffer> <silent> \im :call C_InsertTemplate("idioms.main")
vnoremap <buffer> <silent> \isf :call C_InsertTemplate("idioms.function-static", "v")
onoremap <buffer> <silent> \isf :call C_InsertTemplate("idioms.function-static")
vnoremap <buffer> <silent> \if :call C_InsertTemplate("idioms.function", "v")
onoremap <buffer> <silent> \if :call C_InsertTemplate("idioms.function")
noremap <buffer> <silent> \pp :call C_InsertTemplate("preprocessor.pragma")
noremap <buffer> <silent> \pl :call C_InsertTemplate("preprocessor.line")
noremap <buffer> <silent> \pe :call C_InsertTemplate("preprocessor.error")
noremap <buffer> <silent> \pr0 :call C_PPIf0Remove()
vnoremap <buffer> <silent> \pi0 :call C_PPIf0("v")
onoremap <buffer> <silent> \pi0 :call C_PPIf0("a")2ji
vnoremap <buffer> <silent> \pind :call C_InsertTemplate("preprocessor.ifndef-def-endif", "v")
vnoremap <buffer> <silent> \pin :call C_InsertTemplate("preprocessor.ifndef-else-endif", "v")
vnoremap <buffer> <silent> \pid :call C_InsertTemplate("preprocessor.ifdef-else-endif", "v")
vnoremap <buffer> <silent> \pie :call C_InsertTemplate("preprocessor.if-else-endif", "v")
vnoremap <buffer> <silent> \pif :call C_InsertTemplate("preprocessor.if-endif", "v")
onoremap <buffer> <silent> \pind :call C_InsertTemplate("preprocessor.ifndef-def-endif")
onoremap <buffer> <silent> \pin :call C_InsertTemplate("preprocessor.ifndef-else-endif")
onoremap <buffer> <silent> \pid :call C_InsertTemplate("preprocessor.ifdef-else-endif")
onoremap <buffer> <silent> \pie :call C_InsertTemplate("preprocessor.if-else-endif")
onoremap <buffer> <silent> \pif :call C_InsertTemplate("preprocessor.if-endif")
noremap <buffer> <silent> \pu :call C_InsertTemplate("preprocessor.undefine")
noremap <buffer> <silent> \pd :call C_InsertTemplate("preprocessor.define")
noremap <buffer> <silent> \p" :call C_InsertTemplate("preprocessor.include-local")
noremap <buffer> <silent> \p< :call C_InsertTemplate("preprocessor.include-global")
noremap <buffer> \+pc :IncludeCppCLibrary 
noremap <buffer> \+ps :IncludeCppLibrary 
noremap <buffer> \pc :IncludeC99Library 
noremap <buffer> \ps :IncludeStdLibrary 
vnoremap <buffer> <silent> \sb :call C_InsertTemplate("statements.block", "v")
onoremap <buffer> <silent> \sb :call C_InsertTemplate("statements.block")
vnoremap <buffer> <silent> \s{ :call C_InsertTemplate("statements.block", "v")
onoremap <buffer> <silent> \s{ :call C_InsertTemplate("statements.block")
noremap <buffer> <silent> \sc :call C_InsertTemplate("statements.case")
vnoremap <buffer> <silent> \ss :call C_InsertTemplate("statements.switch", "v")
onoremap <buffer> <silent> \ss :call C_InsertTemplate("statements.switch")
vnoremap <buffer> <silent> \swh :call C_InsertTemplate("statements.while-block", "v")
onoremap <buffer> <silent> \swh :call C_InsertTemplate("statements.while-block")
noremap <buffer> <silent> \sw :call C_InsertTemplate("statements.while")
vnoremap <buffer> <silent> \se :call C_InsertTemplate("statements.else-block", "v")
onoremap <buffer> <silent> \se :call C_InsertTemplate("statements.else-block")
vnoremap <buffer> <silent> \sife :call C_InsertTemplate("statements.if-block-else", "v")
onoremap <buffer> <silent> \sife :call C_InsertTemplate("statements.if-block-else")
vnoremap <buffer> <silent> \sie :call C_InsertTemplate("statements.if-else", "v")
onoremap <buffer> <silent> \sie :call C_InsertTemplate("statements.if-else")
vnoremap <buffer> <silent> \sif :call C_InsertTemplate("statements.if-block", "v")
onoremap <buffer> <silent> \sif :call C_InsertTemplate("statements.if-block")
noremap <buffer> <silent> \si :call C_InsertTemplate("statements.if")
vnoremap <buffer> <silent> \sfo :call C_InsertTemplate("statements.for-block", "v")
onoremap <buffer> <silent> \sfo :call C_InsertTemplate("statements.for-block")
noremap <buffer> <silent> \sf :call C_InsertTemplate("statements.for")
vnoremap <buffer> <silent> \sd :call C_InsertTemplate("statements.do-while", "v")
onoremap <buffer> <silent> \sd :call C_InsertTemplate("statements.do-while")
noremap <buffer> \csc :SpecialComment 
noremap <buffer> \ckc :KeywordComment 
noremap <buffer> \chs :HFileSection 
noremap <buffer> \ccs :CFileSection 
vnoremap <buffer> <silent> \cx :call C_CommentToggle( )
onoremap <buffer> <silent> \cx :call C_CommentToggle( )
vnoremap <buffer> <silent> \ct s:call C_InsertDateAndTime('dt')a
onoremap <buffer> <silent> \ct :call C_InsertDateAndTime('dt')
vnoremap <buffer> <silent> \cd s:call C_InsertDateAndTime('d')a
onoremap <buffer> <silent> \cd :call C_InsertDateAndTime('d')
noremap <buffer> <silent> \cfdh :call C_InsertTemplate("comment.file-description-header")
noremap <buffer> <silent> \cfdi :call C_InsertTemplate("comment.file-description")
noremap <buffer> <silent> \ccl :call C_InsertTemplate("comment.class")
noremap <buffer> <silent> \cme :call C_InsertTemplate("comment.method")
noremap <buffer> <silent> \cfu :call C_InsertTemplate("comment.function")
noremap <buffer> <silent> \cfr :call C_InsertTemplate("comment.frame")
vnoremap <buffer> <silent> \co :call C_CommentToCode():nohlsearch
onoremap <buffer> <silent> \co :call C_CommentToCode():nohlsearch
vnoremap <buffer> <silent> \cc :call C_CodeToCommentCpp():nohlsearchj
onoremap <buffer> <silent> \cc :call C_CodeToCommentCpp():nohlsearchj
vnoremap <buffer> <silent> \c* :call C_CodeToCommentC():nohlsearchj
onoremap <buffer> <silent> \c* :call C_CodeToCommentC():nohlsearchj
noremap <buffer> <silent> \cs :call C_GetLineEndCommCol()
vnoremap <buffer> <silent> \cj :call C_AdjustLineEndComm()
nnoremap <buffer> <silent> \cj :call C_AdjustLineEndComm()
vnoremap <buffer> <silent> \cl :call C_EndOfLineComment()
onoremap <buffer> <silent> \cl :call C_EndOfLineComment()
map <buffer> <silent> <S-F9> :call C_Arguments()
map <buffer> <silent> <C-F9> :call C_Run()
map <buffer> <silent> <F9> :call C_Link():call C_HlMessage()
map <buffer> <silent> <M-F9> :call C_Compile():call C_HlMessage()
imap <buffer> <silent> <NL> =C_JumpCtrlJ()
imap <buffer> <silent> \hm :call C_Help("m")
imap <buffer> <silent> \hp :call C_HelpCsupport()
imap <buffer> <silent> \ro :call C_Toggle_Gvim_Xterm()
imap <buffer> <silent> \rx :call C_XtermSize()
imap <buffer> <silent> \rs :call C_Settings()
imap <buffer> <silent> \rh :call C_Hardcopy()
imap <buffer> <silent> \rd :call C_Indent()
imap <buffer> <silent> \rpa :call C_SplintArguments()
imap <buffer> <silent> \rp :call C_SplintCheck():call C_HlMessage()
imap <buffer> <silent> \rma :call C_MakeArguments()
imap <buffer> <silent> \rme :call C_ExeToRun()
imap <buffer> <silent> \rmc :call C_MakeClean()
imap <buffer> <silent> \rm :call C_Make()
imap <buffer> <silent> \ra :call C_Arguments()
imap <buffer> <silent> \rr :call C_Run()
imap <buffer> <silent> \rl :call C_Link():call C_HlMessage()
imap <buffer> <silent> \rc :call C_Compile():call C_HlMessage()
inoremap <buffer> <silent> \+c. :call C_InsertTemplate("cpp.catch-points")
inoremap <buffer> <silent> \+ca :call C_InsertTemplate("cpp.catch")
inoremap <buffer> <silent> \+tr :call C_InsertTemplate("cpp.try-catch")
inoremap <buffer> <silent> \+ec :call C_InsertTemplate("cpp.error-class")
inoremap <buffer> <silent> \+tf :call C_InsertTemplate("cpp.template-function")
inoremap <buffer> <silent> \+tai :call C_InsertTemplate("cpp.template-accessor-implementation")
inoremap <buffer> <silent> \+tmi :call C_InsertTemplate("cpp.template-method-implementation")
inoremap <buffer> <silent> \+tcni :call C_InsertTemplate("cpp.template-class-using-new-implementation")
inoremap <buffer> <silent> \+tci :call C_InsertTemplate("cpp.template-class-implementation")
inoremap <buffer> <silent> \+tcn :call C_InsertTemplate("cpp.template-class-using-new-definition")
inoremap <buffer> <silent> \+tc :call C_InsertTemplate("cpp.template-class-definition")
inoremap <buffer> <silent> \+ai :call C_InsertTemplate("cpp.accessor-implementation")
inoremap <buffer> <silent> \+mi :call C_InsertTemplate("cpp.method-implementation")
inoremap <buffer> <silent> \+cni :call C_InsertTemplate("cpp.class-using-new-implementation")
inoremap <buffer> <silent> \+ci :call C_InsertTemplate("cpp.class-implementation")
inoremap <buffer> <silent> \+cn :call C_InsertTemplate("cpp.class-using-new-definition")
inoremap <buffer> <silent> \+c :call C_InsertTemplate("cpp.class-definition")
inoremap <buffer> <silent> \+co :call C_InsertTemplate("cpp.cout")
inoremap <buffer> <silent> \+" :call C_InsertTemplate("cpp.cout-operator")
inoremap <buffer> \nts :CStyle 
inoremap <buffer> <silent> \ntr :call C_RereadTemplates()
inoremap <buffer> <silent> \ntl :call C_BrowseTemplateFiles("Local")
inoremap <buffer> <silent> \ns :call C_ProtoShow()
inoremap <buffer> <silent> \nc :call C_ProtoClear()
inoremap <buffer> <silent> \ni :call C_ProtoInsert()
inoremap <buffer> <silent> \nm :call C_ProtoPick("method")
inoremap <buffer> <silent> \nf :call C_ProtoPick("function")
inoremap <buffer> <silent> \np :call C_ProtoPick("function")
inoremap <buffer> <silent> \ne :call C_CodeSnippet("e")
inoremap <buffer> <silent> \nw :call C_CodeSnippet("w")
inoremap <buffer> <silent> \nr :call C_CodeSnippet("r")
inoremap <buffer> <silent> \ifp :call C_InsertTemplate("idioms.fprintf")
inoremap <buffer> <silent> \ifs :call C_InsertTemplate("idioms.fscanf")
inoremap <buffer> <silent> \io :call C_InsertTemplate("idioms.open-output-file")
inoremap <buffer> <silent> \ii :call C_InsertTemplate("idioms.open-input-file")
inoremap <buffer> <silent> \ias :call C_InsertTemplate("idioms.assert")
inoremap <buffer> <silent> \isi :call C_InsertTemplate("idioms.sizeof")
inoremap <buffer> <silent> \ire :call C_InsertTemplate("idioms.realloc")
inoremap <buffer> <silent> \ima :call C_InsertTemplate("idioms.malloc")
inoremap <buffer> <silent> \ica :call C_InsertTemplate("idioms.calloc")
inoremap <buffer> <silent> \isc :call C_InsertTemplate("idioms.scanf")
inoremap <buffer> <silent> \ip :call C_InsertTemplate("idioms.printf")
inoremap <buffer> <silent> \iu :call C_InsertTemplate("idioms.union")
inoremap <buffer> <silent> \is :call C_InsertTemplate("idioms.struct")
inoremap <buffer> <silent> \ie :call C_InsertTemplate("idioms.enum")
inoremap <buffer> <silent> \in :call C_CodeFor("down")i
inoremap <buffer> <silent> \i0 :call C_CodeFor("up"  )i
inoremap <buffer> <silent> \im :call C_InsertTemplate("idioms.main")
inoremap <buffer> <silent> \isf :call C_InsertTemplate("idioms.function-static")
inoremap <buffer> <silent> \if :call C_InsertTemplate("idioms.function")
inoremap <buffer> <silent> \pp :call C_InsertTemplate("preprocessor.pragma")
inoremap <buffer> <silent> \pl :call C_InsertTemplate("preprocessor.line")
inoremap <buffer> <silent> \pe :call C_InsertTemplate("preprocessor.error")
inoremap <buffer> <silent> \pr0 :call C_PPIf0Remove()
inoremap <buffer> <silent> \pi0 :call C_PPIf0("a")2ji
inoremap <buffer> <silent> \pind :call C_InsertTemplate("preprocessor.ifndef-def-endif")
inoremap <buffer> <silent> \pin :call C_InsertTemplate("preprocessor.ifndef-else-endif")
inoremap <buffer> <silent> \pid :call C_InsertTemplate("preprocessor.ifdef-else-endif")
inoremap <buffer> <silent> \pie :call C_InsertTemplate("preprocessor.if-else-endif")
inoremap <buffer> <silent> \pif :call C_InsertTemplate("preprocessor.if-endif")
inoremap <buffer> <silent> \pu :call C_InsertTemplate("preprocessor.undefine")
inoremap <buffer> <silent> \pd :call C_InsertTemplate("preprocessor.define")
inoremap <buffer> <silent> \p" :call C_InsertTemplate("preprocessor.include-local")
inoremap <buffer> <silent> \p< :call C_InsertTemplate("preprocessor.include-global")
inoremap <buffer> \+pc :IncludeCppC9Library 
inoremap <buffer> \+ps :IncludeCppLibrary 
inoremap <buffer> \pc :IncludeC99Library 
inoremap <buffer> \ps :IncludeStdLibrary 
inoremap <buffer> <silent> \sb :call C_InsertTemplate("statements.block")
inoremap <buffer> <silent> \s{ :call C_InsertTemplate("statements.block")
inoremap <buffer> <silent> \sc :call C_InsertTemplate("statements.case")
inoremap <buffer> <silent> \ss :call C_InsertTemplate("statements.switch")
inoremap <buffer> <silent> \swh :call C_InsertTemplate("statements.while-block")
inoremap <buffer> <silent> \sw :call C_InsertTemplate("statements.while")
inoremap <buffer> <silent> \se :call C_InsertTemplate("statements.else-block")
inoremap <buffer> <silent> \sife :call C_InsertTemplate("statements.if-block-else")
inoremap <buffer> <silent> \sie :call C_InsertTemplate("statements.if-else")
inoremap <buffer> <silent> \sif :call C_InsertTemplate("statements.if-block")
inoremap <buffer> <silent> \si :call C_InsertTemplate("statements.if")
inoremap <buffer> <silent> \sfo :call C_InsertTemplate("statements.for-block")
inoremap <buffer> <silent> \sf :call C_InsertTemplate("statements.for")
inoremap <buffer> <silent> \sd :call C_InsertTemplate("statements.do-while")
inoremap <buffer> \csc :SpecialComment 
inoremap <buffer> \ckc :KeywordComment 
inoremap <buffer> \chs :HFileSection 
inoremap <buffer> \ccs :CFileSection 
inoremap <buffer> <silent> \cx :call C_CommentToggle( )
inoremap <buffer> <silent> \ct :call C_InsertDateAndTime('dt')a
inoremap <buffer> <silent> \cd :call C_InsertDateAndTime('d')a
inoremap <buffer> <silent> \cfdh :call C_InsertTemplate("comment.file-description-header")
inoremap <buffer> <silent> \cfdi :call C_InsertTemplate("comment.file-description")
inoremap <buffer> <silent> \ccl :call C_InsertTemplate("comment.class")
inoremap <buffer> <silent> \cme :call C_InsertTemplate("comment.method")
inoremap <buffer> <silent> \cfu :call C_InsertTemplate("comment.function")
inoremap <buffer> <silent> \cfr :call C_InsertTemplate("comment.frame")
inoremap <buffer> <silent> \cj :call C_AdjustLineEndComm()a
inoremap <buffer> <silent> \cl :call C_EndOfLineComment()
let &cpo=s:cpo_save
unlet s:cpo_save
setlocal keymap=
setlocal noarabic
setlocal autoindent
setlocal nobinary
setlocal bufhidden=
setlocal buflisted
setlocal buftype=
setlocal nocindent
setlocal cinkeys=0{,0},0),:,0#,!^F,o,O,e
setlocal cinoptions=
setlocal cinwords=if,else,while,do,for,switch
setlocal colorcolumn=
setlocal comments=s1:/*,mb:*,ex:*/,://,b:#,:%,:XCOMM,n:>,fb:-
setlocal commentstring=/*%s*/
setlocal complete=.,w,b,u,t,i
setlocal concealcursor=
setlocal conceallevel=0
setlocal completefunc=
setlocal nocopyindent
setlocal cryptmethod=
setlocal nocursorbind
setlocal nocursorcolumn
setlocal nocursorline
setlocal define=
setlocal dictionary=
setlocal nodiff
setlocal equalprg=
setlocal errorformat=%*[^\"]\"%f\"%*\\D%l:%c:\ %m,%*[^\"]\"%f\"%*\\D%l:\ %m,\"%f\"%*\\D%l:%c:\ %m,\"%f\"%*\\D%l:\ %m,%-G%f:%l:\ %trror:\ (Each\ undeclared\ identifier\ is\ reported\ only\ once,%-G%f:%l:\ %trror:\ for\ each\ function\ it\ appears\ in.),%f:%l:%c:\ %trror:\ %m,%f:%l:%c:\ %tarning:\ %m,%f:%l:%c:\ %m,%f:%l:\ %trror:\ %m,%f:%l:\ %tarning:\ %m,%f:%l:\ %m,\"%f\"\\,\ line\ %l%*\\D%c%*[^\ ]\ %m,%D%*\\a[%*\\d]:\ Entering\ directory\ `%f',%X%*\\a[%*\\d]:\ Leaving\ directory\ `%f',%D%*\\a:\ Entering\ directory\ `%f',%X%*\\a:\ Leaving\ directory\ `%f',%DMaking\ %*\\a\ in\ %f
setlocal noexpandtab
if &filetype != 'cpp'
setlocal filetype=cpp
endif
setlocal foldcolumn=0
setlocal foldenable
setlocal foldexpr=0
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldmarker={{{,}}}
setlocal foldmethod=manual
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldtext=foldtext()
setlocal formatexpr=
setlocal formatoptions=tcq
setlocal formatlistpat=^\\s*\\d\\+[\\]:.)}\\t\ ]\\s*
setlocal grepprg=
setlocal iminsert=0
setlocal imsearch=0
setlocal include=
setlocal includeexpr=
setlocal indentexpr=
setlocal indentkeys=0{,0},:,0#,!^F,o,O,e
setlocal noinfercase
setlocal iskeyword=@,48-57,_,192-255
setlocal keywordprg=
setlocal nolinebreak
setlocal nolisp
setlocal nolist
setlocal makeprg=
setlocal matchpairs=(:),{:},[:]
setlocal nomodeline
setlocal modifiable
setlocal nrformats=octal,hex
set number
setlocal number
setlocal numberwidth=4
setlocal omnifunc=
setlocal path=
setlocal nopreserveindent
setlocal nopreviewwindow
setlocal quoteescape=\\
setlocal noreadonly
setlocal norelativenumber
setlocal norightleft
setlocal rightleftcmd=search
setlocal noscrollbind
setlocal shiftwidth=4
setlocal noshortname
setlocal nosmartindent
setlocal softtabstop=0
setlocal nospell
setlocal spellcapcheck=[.?!]\\_[\\])'\"\	\ ]\\+
setlocal spellfile=
setlocal spelllang=en
setlocal statusline=
setlocal suffixesadd=
setlocal swapfile
setlocal synmaxcol=3000
if &syntax != 'cpp'
setlocal syntax=cpp
endif
setlocal tabstop=4
setlocal tags=
setlocal textwidth=0
setlocal thesaurus=
setlocal noundofile
setlocal nowinfixheight
setlocal nowinfixwidth
setlocal wrap
setlocal wrapmargin=0
silent! normal! zE
let s:l = 304 - ((6 * winheight(0) + 12) / 24)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
304
normal! 096l
wincmd w
argglobal
edit cached.cpp
let s:cpo_save=&cpo
set cpo&vim
imap <buffer> <silent> <S-F9> :call C_Arguments()
imap <buffer> <silent> <C-F9> :call C_Run()
imap <buffer> <silent> <F9> :call C_Link():call C_HlMessage()
imap <buffer> <silent> <M-F9> :call C_Compile():call C_HlMessage()
nmap <buffer> <silent> <NL> i=C_JumpCtrlJ()
nnoremap <buffer> <silent> \cl :call C_EndOfLineComment()
nnoremap <buffer> <silent> \c* :call C_CodeToCommentC():nohlsearchj
nnoremap <buffer> <silent> \cc :call C_CodeToCommentCpp():nohlsearchj
nnoremap <buffer> <silent> \co :call C_CommentToCode():nohlsearch
nnoremap <buffer> <silent> \cd :call C_InsertDateAndTime('d')
nnoremap <buffer> <silent> \ct :call C_InsertDateAndTime('dt')
nnoremap <buffer> <silent> \cx :call C_CommentToggle( )
nnoremap <buffer> <silent> \sd :call C_InsertTemplate("statements.do-while")
nnoremap <buffer> <silent> \sfo :call C_InsertTemplate("statements.for-block")
nnoremap <buffer> <silent> \sif :call C_InsertTemplate("statements.if-block")
nnoremap <buffer> <silent> \sie :call C_InsertTemplate("statements.if-else")
nnoremap <buffer> <silent> \sife :call C_InsertTemplate("statements.if-block-else")
nnoremap <buffer> <silent> \se :call C_InsertTemplate("statements.else-block")
nnoremap <buffer> <silent> \swh :call C_InsertTemplate("statements.while-block")
nnoremap <buffer> <silent> \ss :call C_InsertTemplate("statements.switch")
nnoremap <buffer> <silent> \s{ :call C_InsertTemplate("statements.block")
nnoremap <buffer> <silent> \sb :call C_InsertTemplate("statements.block")
nnoremap <buffer> <silent> \pif :call C_InsertTemplate("preprocessor.if-endif")
nnoremap <buffer> <silent> \pie :call C_InsertTemplate("preprocessor.if-else-endif")
nnoremap <buffer> <silent> \pid :call C_InsertTemplate("preprocessor.ifdef-else-endif")
nnoremap <buffer> <silent> \pin :call C_InsertTemplate("preprocessor.ifndef-else-endif")
nnoremap <buffer> <silent> \pind :call C_InsertTemplate("preprocessor.ifndef-def-endif")
nnoremap <buffer> <silent> \pi0 :call C_PPIf0("a")2ji
nnoremap <buffer> <silent> \if :call C_InsertTemplate("idioms.function")
nnoremap <buffer> <silent> \isf :call C_InsertTemplate("idioms.function-static")
nnoremap <buffer> <silent> \im :call C_InsertTemplate("idioms.main")
nnoremap <buffer> <silent> \i0 :call C_CodeFor("up"  )
nnoremap <buffer> <silent> \in :call C_CodeFor("down")
nnoremap <buffer> <silent> \ie :call C_InsertTemplate("idioms.enum")
nnoremap <buffer> <silent> \is :call C_InsertTemplate("idioms.struct")
nnoremap <buffer> <silent> \iu :call C_InsertTemplate("idioms.union")
nnoremap <buffer> <silent> \isi :call C_InsertTemplate("idioms.sizeof")
nnoremap <buffer> <silent> \ias :call C_InsertTemplate("idioms.assert")
nnoremap <buffer> <silent> \ii :call C_InsertTemplate("idioms.open-input-file")
nnoremap <buffer> <silent> \io :call C_InsertTemplate("idioms.open-output-file")
nnoremap <buffer> <silent> \nw :call C_CodeSnippet("w")
nnoremap <buffer> <silent> \np :call C_ProtoPick("function")
nnoremap <buffer> <silent> \nf :call C_ProtoPick("function")
nnoremap <buffer> <silent> \nm :call C_ProtoPick("method")
nnoremap <buffer> <silent> \+tr :call C_InsertTemplate("cpp.try-catch")
nnoremap <buffer> <silent> \+ca :call C_InsertTemplate("cpp.catch")
nnoremap <buffer> <silent> \+c. :call C_InsertTemplate("cpp.catch-points")
nmap <buffer> <silent> \rh :call C_Hardcopy()
vmap <buffer> <silent> \rh :call C_Hardcopy()
vnoremap <buffer> <silent> \+c. :call C_InsertTemplate("cpp.catch-points", "v")
vnoremap <buffer> <silent> \+ca :call C_InsertTemplate("cpp.catch", "v")
vnoremap <buffer> <silent> \+tr :call C_InsertTemplate("cpp.try-catch", "v")
vnoremap <buffer> <silent> \nm :call C_ProtoPick("method")
vnoremap <buffer> <silent> \nf :call C_ProtoPick("function")
vnoremap <buffer> <silent> \np :call C_ProtoPick("function")
vnoremap <buffer> <silent> \nw :call C_CodeSnippet("wv")
vnoremap <buffer> <silent> \io :call C_InsertTemplate("idioms.open-output-file", "v")
vnoremap <buffer> <silent> \ii :call C_InsertTemplate("idioms.open-input-file", "v")
vnoremap <buffer> <silent> \ias :call C_InsertTemplate("idioms.assert", "v")
vnoremap <buffer> <silent> \isi :call C_InsertTemplate("idioms.sizeof", "v")
vnoremap <buffer> <silent> \iu :call C_InsertTemplate("idioms.union" , "v")
vnoremap <buffer> <silent> \is :call C_InsertTemplate("idioms.struct", "v")
vnoremap <buffer> <silent> \ie :call C_InsertTemplate("idioms.enum"  , "v")
vnoremap <buffer> <silent> \in :call C_CodeFor("down")
vnoremap <buffer> <silent> \i0 :call C_CodeFor("up"  )
vnoremap <buffer> <silent> \im :call C_InsertTemplate("idioms.main", "v")
vnoremap <buffer> <silent> \isf :call C_InsertTemplate("idioms.function-static", "v")
vnoremap <buffer> <silent> \if :call C_InsertTemplate("idioms.function", "v")
vnoremap <buffer> <silent> \pi0 :call C_PPIf0("v")
vnoremap <buffer> <silent> \pind :call C_InsertTemplate("preprocessor.ifndef-def-endif", "v")
vnoremap <buffer> <silent> \pin :call C_InsertTemplate("preprocessor.ifndef-else-endif", "v")
vnoremap <buffer> <silent> \pid :call C_InsertTemplate("preprocessor.ifdef-else-endif", "v")
vnoremap <buffer> <silent> \pie :call C_InsertTemplate("preprocessor.if-else-endif", "v")
vnoremap <buffer> <silent> \pif :call C_InsertTemplate("preprocessor.if-endif", "v")
vnoremap <buffer> <silent> \sb :call C_InsertTemplate("statements.block", "v")
vnoremap <buffer> <silent> \s{ :call C_InsertTemplate("statements.block", "v")
vnoremap <buffer> <silent> \ss :call C_InsertTemplate("statements.switch", "v")
vnoremap <buffer> <silent> \swh :call C_InsertTemplate("statements.while-block", "v")
vnoremap <buffer> <silent> \se :call C_InsertTemplate("statements.else-block", "v")
vnoremap <buffer> <silent> \sife :call C_InsertTemplate("statements.if-block-else", "v")
vnoremap <buffer> <silent> \sie :call C_InsertTemplate("statements.if-else", "v")
vnoremap <buffer> <silent> \sif :call C_InsertTemplate("statements.if-block", "v")
vnoremap <buffer> <silent> \sfo :call C_InsertTemplate("statements.for-block", "v")
vnoremap <buffer> <silent> \sd :call C_InsertTemplate("statements.do-while", "v")
vnoremap <buffer> <silent> \cx :call C_CommentToggle( )
vnoremap <buffer> <silent> \ct s:call C_InsertDateAndTime('dt')a
vnoremap <buffer> <silent> \cd s:call C_InsertDateAndTime('d')a
vnoremap <buffer> <silent> \co :call C_CommentToCode():nohlsearch
vnoremap <buffer> <silent> \cc :call C_CodeToCommentCpp():nohlsearchj
vnoremap <buffer> <silent> \c* :call C_CodeToCommentC():nohlsearchj
vnoremap <buffer> <silent> \cl :call C_EndOfLineComment()
onoremap <buffer> <silent> \cl :call C_EndOfLineComment()
onoremap <buffer> <silent> \c* :call C_CodeToCommentC():nohlsearchj
onoremap <buffer> <silent> \cc :call C_CodeToCommentCpp():nohlsearchj
onoremap <buffer> <silent> \co :call C_CommentToCode():nohlsearch
onoremap <buffer> <silent> \cd :call C_InsertDateAndTime('d')
onoremap <buffer> <silent> \ct :call C_InsertDateAndTime('dt')
onoremap <buffer> <silent> \cx :call C_CommentToggle( )
onoremap <buffer> <silent> \sd :call C_InsertTemplate("statements.do-while")
onoremap <buffer> <silent> \sfo :call C_InsertTemplate("statements.for-block")
onoremap <buffer> <silent> \sif :call C_InsertTemplate("statements.if-block")
onoremap <buffer> <silent> \sie :call C_InsertTemplate("statements.if-else")
onoremap <buffer> <silent> \sife :call C_InsertTemplate("statements.if-block-else")
onoremap <buffer> <silent> \se :call C_InsertTemplate("statements.else-block")
onoremap <buffer> <silent> \swh :call C_InsertTemplate("statements.while-block")
onoremap <buffer> <silent> \ss :call C_InsertTemplate("statements.switch")
onoremap <buffer> <silent> \s{ :call C_InsertTemplate("statements.block")
onoremap <buffer> <silent> \sb :call C_InsertTemplate("statements.block")
onoremap <buffer> <silent> \pif :call C_InsertTemplate("preprocessor.if-endif")
onoremap <buffer> <silent> \pie :call C_InsertTemplate("preprocessor.if-else-endif")
onoremap <buffer> <silent> \pid :call C_InsertTemplate("preprocessor.ifdef-else-endif")
onoremap <buffer> <silent> \pin :call C_InsertTemplate("preprocessor.ifndef-else-endif")
onoremap <buffer> <silent> \pind :call C_InsertTemplate("preprocessor.ifndef-def-endif")
onoremap <buffer> <silent> \pi0 :call C_PPIf0("a")2ji
onoremap <buffer> <silent> \if :call C_InsertTemplate("idioms.function")
onoremap <buffer> <silent> \isf :call C_InsertTemplate("idioms.function-static")
onoremap <buffer> <silent> \im :call C_InsertTemplate("idioms.main")
onoremap <buffer> <silent> \i0 :call C_CodeFor("up"  )
onoremap <buffer> <silent> \in :call C_CodeFor("down")
onoremap <buffer> <silent> \ie :call C_InsertTemplate("idioms.enum")
onoremap <buffer> <silent> \is :call C_InsertTemplate("idioms.struct")
onoremap <buffer> <silent> \iu :call C_InsertTemplate("idioms.union")
onoremap <buffer> <silent> \isi :call C_InsertTemplate("idioms.sizeof")
onoremap <buffer> <silent> \ias :call C_InsertTemplate("idioms.assert")
onoremap <buffer> <silent> \ii :call C_InsertTemplate("idioms.open-input-file")
onoremap <buffer> <silent> \io :call C_InsertTemplate("idioms.open-output-file")
onoremap <buffer> <silent> \nw :call C_CodeSnippet("w")
onoremap <buffer> <silent> \np :call C_ProtoPick("function")
onoremap <buffer> <silent> \nf :call C_ProtoPick("function")
onoremap <buffer> <silent> \nm :call C_ProtoPick("method")
onoremap <buffer> <silent> \+tr :call C_InsertTemplate("cpp.try-catch")
onoremap <buffer> <silent> \+ca :call C_InsertTemplate("cpp.catch")
onoremap <buffer> <silent> \+c. :call C_InsertTemplate("cpp.catch-points")
omap <buffer> <silent> \rh :call C_Hardcopy()
map <buffer> <silent> \hm :call C_Help("m")
map <buffer> <silent> \hp :call C_HelpCsupport()
map <buffer> <silent> \ro :call C_Toggle_Gvim_Xterm()
map <buffer> <silent> \rx :call C_XtermSize()
map <buffer> <silent> \rs :call C_Settings()
map <buffer> <silent> \rd :call C_Indent()
map <buffer> <silent> \rpa :call C_SplintArguments()
map <buffer> <silent> \rp :call C_SplintCheck():call C_HlMessage()
map <buffer> <silent> \rma :call C_MakeArguments()
map <buffer> <silent> \rme :call C_ExeToRun()
map <buffer> <silent> \rmc :call C_MakeClean()
map <buffer> <silent> \rcm :call C_ChooseMakefile()
map <buffer> <silent> \rm :call C_Make()
map <buffer> <silent> \ra :call C_Arguments()
map <buffer> <silent> \rr :call C_Run()
map <buffer> <silent> \rl :call C_Link():call C_HlMessage()
map <buffer> <silent> \rc :call C_Compile():call C_HlMessage()
noremap <buffer> <silent> \+ec :call C_InsertTemplate("cpp.error-class")
noremap <buffer> <silent> \+tf :call C_InsertTemplate("cpp.template-function")
noremap <buffer> <silent> \+tai :call C_InsertTemplate("cpp.template-accessor-implementation")
noremap <buffer> <silent> \+tmi :call C_InsertTemplate("cpp.template-method-implementation")
noremap <buffer> <silent> \+tcni :call C_InsertTemplate("cpp.template-class-using-new-implementation")
noremap <buffer> <silent> \+tci :call C_InsertTemplate("cpp.template-class-implementation")
noremap <buffer> <silent> \+tcn :call C_InsertTemplate("cpp.template-class-using-new-definition")
noremap <buffer> <silent> \+tc :call C_InsertTemplate("cpp.template-class-definition")
noremap <buffer> <silent> \+ai :call C_InsertTemplate("cpp.accessor-implementation")
noremap <buffer> <silent> \+mi :call C_InsertTemplate("cpp.method-implementation")
noremap <buffer> <silent> \+cni :call C_InsertTemplate("cpp.class-using-new-implementation")
noremap <buffer> <silent> \+ci :call C_InsertTemplate("cpp.class-implementation")
noremap <buffer> <silent> \+cn :call C_InsertTemplate("cpp.class-using-new-definition")
noremap <buffer> <silent> \+c :call C_InsertTemplate("cpp.class-definition")
noremap <buffer> <silent> \+co :call C_InsertTemplate("cpp.cout")
noremap <buffer> <silent> \+" :call C_InsertTemplate("cpp.cout-operator")
noremap <buffer> \nts :CStyle 
noremap <buffer> <silent> \ntr :call C_RereadTemplates()
noremap <buffer> <silent> \ntl :call C_BrowseTemplateFiles("Local")
noremap <buffer> <silent> \ns :call C_ProtoShow()
noremap <buffer> <silent> \nc :call C_ProtoClear()
noremap <buffer> <silent> \ni :call C_ProtoInsert()
noremap <buffer> <silent> \ne :call C_CodeSnippet("e")
noremap <buffer> <silent> \nr :call C_CodeSnippet("r")
noremap <buffer> <silent> \ifp :call C_InsertTemplate("idioms.fprintf")
noremap <buffer> <silent> \ifs :call C_InsertTemplate("idioms.fscanf")
noremap <buffer> <silent> \ire :call C_InsertTemplate("idioms.realloc")
noremap <buffer> <silent> \ima :call C_InsertTemplate("idioms.malloc")
noremap <buffer> <silent> \ica :call C_InsertTemplate("idioms.calloc")
noremap <buffer> <silent> \isc :call C_InsertTemplate("idioms.scanf")
noremap <buffer> <silent> \ip :call C_InsertTemplate("idioms.printf")
noremap <buffer> <silent> \pp :call C_InsertTemplate("preprocessor.pragma")
noremap <buffer> <silent> \pl :call C_InsertTemplate("preprocessor.line")
noremap <buffer> <silent> \pe :call C_InsertTemplate("preprocessor.error")
noremap <buffer> <silent> \pr0 :call C_PPIf0Remove()
noremap <buffer> <silent> \pu :call C_InsertTemplate("preprocessor.undefine")
noremap <buffer> <silent> \pd :call C_InsertTemplate("preprocessor.define")
noremap <buffer> <silent> \p" :call C_InsertTemplate("preprocessor.include-local")
noremap <buffer> <silent> \p< :call C_InsertTemplate("preprocessor.include-global")
noremap <buffer> \+pc :IncludeCppCLibrary 
noremap <buffer> \+ps :IncludeCppLibrary 
noremap <buffer> \pc :IncludeC99Library 
noremap <buffer> \ps :IncludeStdLibrary 
noremap <buffer> <silent> \sc :call C_InsertTemplate("statements.case")
noremap <buffer> <silent> \sw :call C_InsertTemplate("statements.while")
noremap <buffer> <silent> \si :call C_InsertTemplate("statements.if")
noremap <buffer> <silent> \sf :call C_InsertTemplate("statements.for")
noremap <buffer> \csc :SpecialComment 
noremap <buffer> \ckc :KeywordComment 
noremap <buffer> \chs :HFileSection 
noremap <buffer> \ccs :CFileSection 
noremap <buffer> <silent> \cfdh :call C_InsertTemplate("comment.file-description-header")
noremap <buffer> <silent> \cfdi :call C_InsertTemplate("comment.file-description")
noremap <buffer> <silent> \ccl :call C_InsertTemplate("comment.class")
noremap <buffer> <silent> \cme :call C_InsertTemplate("comment.method")
noremap <buffer> <silent> \cfu :call C_InsertTemplate("comment.function")
noremap <buffer> <silent> \cfr :call C_InsertTemplate("comment.frame")
noremap <buffer> <silent> \cs :call C_GetLineEndCommCol()
vnoremap <buffer> <silent> \cj :call C_AdjustLineEndComm()
nnoremap <buffer> <silent> \cj :call C_AdjustLineEndComm()
map <buffer> <silent> <S-F9> :call C_Arguments()
map <buffer> <silent> <C-F9> :call C_Run()
map <buffer> <silent> <F9> :call C_Link():call C_HlMessage()
map <buffer> <silent> <M-F9> :call C_Compile():call C_HlMessage()
imap <buffer> <silent> <NL> =C_JumpCtrlJ()
imap <buffer> <silent> \hm :call C_Help("m")
imap <buffer> <silent> \hp :call C_HelpCsupport()
imap <buffer> <silent> \ro :call C_Toggle_Gvim_Xterm()
imap <buffer> <silent> \rx :call C_XtermSize()
imap <buffer> <silent> \rs :call C_Settings()
imap <buffer> <silent> \rh :call C_Hardcopy()
imap <buffer> <silent> \rd :call C_Indent()
imap <buffer> <silent> \rpa :call C_SplintArguments()
imap <buffer> <silent> \rp :call C_SplintCheck():call C_HlMessage()
imap <buffer> <silent> \rma :call C_MakeArguments()
imap <buffer> <silent> \rme :call C_ExeToRun()
imap <buffer> <silent> \rmc :call C_MakeClean()
imap <buffer> <silent> \rm :call C_Make()
imap <buffer> <silent> \ra :call C_Arguments()
imap <buffer> <silent> \rr :call C_Run()
imap <buffer> <silent> \rl :call C_Link():call C_HlMessage()
imap <buffer> <silent> \rc :call C_Compile():call C_HlMessage()
inoremap <buffer> <silent> \+c. :call C_InsertTemplate("cpp.catch-points")
inoremap <buffer> <silent> \+ca :call C_InsertTemplate("cpp.catch")
inoremap <buffer> <silent> \+tr :call C_InsertTemplate("cpp.try-catch")
inoremap <buffer> <silent> \+ec :call C_InsertTemplate("cpp.error-class")
inoremap <buffer> <silent> \+tf :call C_InsertTemplate("cpp.template-function")
inoremap <buffer> <silent> \+tai :call C_InsertTemplate("cpp.template-accessor-implementation")
inoremap <buffer> <silent> \+tmi :call C_InsertTemplate("cpp.template-method-implementation")
inoremap <buffer> <silent> \+tcni :call C_InsertTemplate("cpp.template-class-using-new-implementation")
inoremap <buffer> <silent> \+tci :call C_InsertTemplate("cpp.template-class-implementation")
inoremap <buffer> <silent> \+tcn :call C_InsertTemplate("cpp.template-class-using-new-definition")
inoremap <buffer> <silent> \+tc :call C_InsertTemplate("cpp.template-class-definition")
inoremap <buffer> <silent> \+ai :call C_InsertTemplate("cpp.accessor-implementation")
inoremap <buffer> <silent> \+mi :call C_InsertTemplate("cpp.method-implementation")
inoremap <buffer> <silent> \+cni :call C_InsertTemplate("cpp.class-using-new-implementation")
inoremap <buffer> <silent> \+ci :call C_InsertTemplate("cpp.class-implementation")
inoremap <buffer> <silent> \+cn :call C_InsertTemplate("cpp.class-using-new-definition")
inoremap <buffer> <silent> \+c :call C_InsertTemplate("cpp.class-definition")
inoremap <buffer> <silent> \+co :call C_InsertTemplate("cpp.cout")
inoremap <buffer> <silent> \+" :call C_InsertTemplate("cpp.cout-operator")
inoremap <buffer> \nts :CStyle 
inoremap <buffer> <silent> \ntr :call C_RereadTemplates()
inoremap <buffer> <silent> \ntl :call C_BrowseTemplateFiles("Local")
inoremap <buffer> <silent> \ns :call C_ProtoShow()
inoremap <buffer> <silent> \nc :call C_ProtoClear()
inoremap <buffer> <silent> \ni :call C_ProtoInsert()
inoremap <buffer> <silent> \nm :call C_ProtoPick("method")
inoremap <buffer> <silent> \nf :call C_ProtoPick("function")
inoremap <buffer> <silent> \np :call C_ProtoPick("function")
inoremap <buffer> <silent> \ne :call C_CodeSnippet("e")
inoremap <buffer> <silent> \nw :call C_CodeSnippet("w")
inoremap <buffer> <silent> \nr :call C_CodeSnippet("r")
inoremap <buffer> <silent> \ifp :call C_InsertTemplate("idioms.fprintf")
inoremap <buffer> <silent> \ifs :call C_InsertTemplate("idioms.fscanf")
inoremap <buffer> <silent> \io :call C_InsertTemplate("idioms.open-output-file")
inoremap <buffer> <silent> \ii :call C_InsertTemplate("idioms.open-input-file")
inoremap <buffer> <silent> \ias :call C_InsertTemplate("idioms.assert")
inoremap <buffer> <silent> \isi :call C_InsertTemplate("idioms.sizeof")
inoremap <buffer> <silent> \ire :call C_InsertTemplate("idioms.realloc")
inoremap <buffer> <silent> \ima :call C_InsertTemplate("idioms.malloc")
inoremap <buffer> <silent> \ica :call C_InsertTemplate("idioms.calloc")
inoremap <buffer> <silent> \isc :call C_InsertTemplate("idioms.scanf")
inoremap <buffer> <silent> \ip :call C_InsertTemplate("idioms.printf")
inoremap <buffer> <silent> \iu :call C_InsertTemplate("idioms.union")
inoremap <buffer> <silent> \is :call C_InsertTemplate("idioms.struct")
inoremap <buffer> <silent> \ie :call C_InsertTemplate("idioms.enum")
inoremap <buffer> <silent> \in :call C_CodeFor("down")i
inoremap <buffer> <silent> \i0 :call C_CodeFor("up"  )i
inoremap <buffer> <silent> \im :call C_InsertTemplate("idioms.main")
inoremap <buffer> <silent> \isf :call C_InsertTemplate("idioms.function-static")
inoremap <buffer> <silent> \if :call C_InsertTemplate("idioms.function")
inoremap <buffer> <silent> \pp :call C_InsertTemplate("preprocessor.pragma")
inoremap <buffer> <silent> \pl :call C_InsertTemplate("preprocessor.line")
inoremap <buffer> <silent> \pe :call C_InsertTemplate("preprocessor.error")
inoremap <buffer> <silent> \pr0 :call C_PPIf0Remove()
inoremap <buffer> <silent> \pi0 :call C_PPIf0("a")2ji
inoremap <buffer> <silent> \pind :call C_InsertTemplate("preprocessor.ifndef-def-endif")
inoremap <buffer> <silent> \pin :call C_InsertTemplate("preprocessor.ifndef-else-endif")
inoremap <buffer> <silent> \pid :call C_InsertTemplate("preprocessor.ifdef-else-endif")
inoremap <buffer> <silent> \pie :call C_InsertTemplate("preprocessor.if-else-endif")
inoremap <buffer> <silent> \pif :call C_InsertTemplate("preprocessor.if-endif")
inoremap <buffer> <silent> \pu :call C_InsertTemplate("preprocessor.undefine")
inoremap <buffer> <silent> \pd :call C_InsertTemplate("preprocessor.define")
inoremap <buffer> <silent> \p" :call C_InsertTemplate("preprocessor.include-local")
inoremap <buffer> <silent> \p< :call C_InsertTemplate("preprocessor.include-global")
inoremap <buffer> \+pc :IncludeCppC9Library 
inoremap <buffer> \+ps :IncludeCppLibrary 
inoremap <buffer> \pc :IncludeC99Library 
inoremap <buffer> \ps :IncludeStdLibrary 
inoremap <buffer> <silent> \sb :call C_InsertTemplate("statements.block")
inoremap <buffer> <silent> \s{ :call C_InsertTemplate("statements.block")
inoremap <buffer> <silent> \sc :call C_InsertTemplate("statements.case")
inoremap <buffer> <silent> \ss :call C_InsertTemplate("statements.switch")
inoremap <buffer> <silent> \swh :call C_InsertTemplate("statements.while-block")
inoremap <buffer> <silent> \sw :call C_InsertTemplate("statements.while")
inoremap <buffer> <silent> \se :call C_InsertTemplate("statements.else-block")
inoremap <buffer> <silent> \sife :call C_InsertTemplate("statements.if-block-else")
inoremap <buffer> <silent> \sie :call C_InsertTemplate("statements.if-else")
inoremap <buffer> <silent> \sif :call C_InsertTemplate("statements.if-block")
inoremap <buffer> <silent> \si :call C_InsertTemplate("statements.if")
inoremap <buffer> <silent> \sfo :call C_InsertTemplate("statements.for-block")
inoremap <buffer> <silent> \sf :call C_InsertTemplate("statements.for")
inoremap <buffer> <silent> \sd :call C_InsertTemplate("statements.do-while")
inoremap <buffer> \csc :SpecialComment 
inoremap <buffer> \ckc :KeywordComment 
inoremap <buffer> \chs :HFileSection 
inoremap <buffer> \ccs :CFileSection 
inoremap <buffer> <silent> \cx :call C_CommentToggle( )
inoremap <buffer> <silent> \ct :call C_InsertDateAndTime('dt')a
inoremap <buffer> <silent> \cd :call C_InsertDateAndTime('d')a
inoremap <buffer> <silent> \cfdh :call C_InsertTemplate("comment.file-description-header")
inoremap <buffer> <silent> \cfdi :call C_InsertTemplate("comment.file-description")
inoremap <buffer> <silent> \ccl :call C_InsertTemplate("comment.class")
inoremap <buffer> <silent> \cme :call C_InsertTemplate("comment.method")
inoremap <buffer> <silent> \cfu :call C_InsertTemplate("comment.function")
inoremap <buffer> <silent> \cfr :call C_InsertTemplate("comment.frame")
inoremap <buffer> <silent> \cj :call C_AdjustLineEndComm()a
inoremap <buffer> <silent> \cl :call C_EndOfLineComment()
let &cpo=s:cpo_save
unlet s:cpo_save
setlocal keymap=
setlocal noarabic
setlocal autoindent
setlocal nobinary
setlocal bufhidden=
setlocal buflisted
setlocal buftype=
setlocal nocindent
setlocal cinkeys=0{,0},0),:,0#,!^F,o,O,e
setlocal cinoptions=
setlocal cinwords=if,else,while,do,for,switch
setlocal colorcolumn=
setlocal comments=s1:/*,mb:*,ex:*/,://,b:#,:%,:XCOMM,n:>,fb:-
setlocal commentstring=/*%s*/
setlocal complete=.,w,b,u,t,i
setlocal concealcursor=
setlocal conceallevel=0
setlocal completefunc=
setlocal nocopyindent
setlocal cryptmethod=
setlocal nocursorbind
setlocal nocursorcolumn
setlocal nocursorline
setlocal define=
setlocal dictionary=~/.vim/c-support/wordlists/c-c++-keywords.list,~/.vim/c-support/wordlists/k+r.list,~/.vim/c-support/wordlists/stl_index.list
setlocal nodiff
setlocal equalprg=
setlocal errorformat=
setlocal noexpandtab
if &filetype != 'cpp'
setlocal filetype=cpp
endif
setlocal foldcolumn=0
setlocal foldenable
setlocal foldexpr=0
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldmarker={{{,}}}
setlocal foldmethod=manual
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldtext=foldtext()
setlocal formatexpr=
setlocal formatoptions=tcq
setlocal formatlistpat=^\\s*\\d\\+[\\]:.)}\\t\ ]\\s*
setlocal grepprg=
setlocal iminsert=0
setlocal imsearch=0
setlocal include=
setlocal includeexpr=
setlocal indentexpr=
setlocal indentkeys=0{,0},:,0#,!^F,o,O,e
setlocal noinfercase
setlocal iskeyword=@,48-57,_,192-255
setlocal keywordprg=
setlocal nolinebreak
setlocal nolisp
setlocal nolist
setlocal makeprg=
setlocal matchpairs=(:),{:},[:]
setlocal nomodeline
setlocal modifiable
setlocal nrformats=octal,hex
set number
setlocal number
setlocal numberwidth=4
setlocal omnifunc=
setlocal path=
setlocal nopreserveindent
setlocal nopreviewwindow
setlocal quoteescape=\\
setlocal noreadonly
setlocal norelativenumber
setlocal norightleft
setlocal rightleftcmd=search
setlocal noscrollbind
setlocal shiftwidth=4
setlocal noshortname
setlocal nosmartindent
setlocal softtabstop=0
setlocal nospell
setlocal spellcapcheck=[.?!]\\_[\\])'\"\	\ ]\\+
setlocal spellfile=
setlocal spelllang=en
setlocal statusline=
setlocal suffixesadd=
setlocal swapfile
setlocal synmaxcol=3000
if &syntax != 'cpp'
setlocal syntax=cpp
endif
setlocal tabstop=4
setlocal tags=
setlocal textwidth=0
setlocal thesaurus=
setlocal noundofile
setlocal nowinfixheight
setlocal nowinfixwidth
setlocal wrap
setlocal wrapmargin=0
silent! normal! zE
let s:l = 1881 - ((13 * winheight(0) + 11) / 23)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
1881
normal! 0
wincmd w
argglobal
enew
setlocal keymap=
setlocal noarabic
setlocal autoindent
setlocal nobinary
setlocal bufhidden=wipe
setlocal buflisted
setlocal buftype=quickfix
setlocal nocindent
setlocal cinkeys=0{,0},0),:,0#,!^F,o,O,e
setlocal cinoptions=
setlocal cinwords=if,else,while,do,for,switch
setlocal colorcolumn=
setlocal comments=s1:/*,mb:*,ex:*/,://,b:#,:%,:XCOMM,n:>,fb:-
setlocal commentstring=/*%s*/
setlocal complete=.,w,b,u,t,i
setlocal concealcursor=
setlocal conceallevel=0
setlocal completefunc=
setlocal nocopyindent
setlocal cryptmethod=
setlocal nocursorbind
setlocal nocursorcolumn
setlocal nocursorline
setlocal define=
setlocal dictionary=
setlocal nodiff
setlocal equalprg=
setlocal errorformat=
setlocal noexpandtab
if &filetype != 'qf'
setlocal filetype=qf
endif
setlocal foldcolumn=0
setlocal foldenable
setlocal foldexpr=0
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldmarker={{{,}}}
setlocal foldmethod=manual
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldtext=foldtext()
setlocal formatexpr=
setlocal formatoptions=tcq
setlocal formatlistpat=^\\s*\\d\\+[\\]:.)}\\t\ ]\\s*
setlocal grepprg=
setlocal iminsert=0
setlocal imsearch=0
setlocal include=
setlocal includeexpr=
setlocal indentexpr=
setlocal indentkeys=0{,0},:,0#,!^F,o,O,e
setlocal noinfercase
setlocal iskeyword=@,48-57,_,192-255
setlocal keywordprg=
setlocal linebreak
setlocal nolisp
setlocal nolist
setlocal makeprg=
setlocal matchpairs=(:),{:},[:]
setlocal nomodeline
setlocal nomodifiable
setlocal nrformats=octal,hex
set number
setlocal number
setlocal numberwidth=4
setlocal omnifunc=
setlocal path=
setlocal nopreserveindent
setlocal nopreviewwindow
setlocal quoteescape=\\
setlocal noreadonly
setlocal norelativenumber
setlocal norightleft
setlocal rightleftcmd=search
setlocal noscrollbind
setlocal shiftwidth=4
setlocal noshortname
setlocal nosmartindent
setlocal softtabstop=0
setlocal nospell
setlocal spellcapcheck=[.?!]\\_[\\])'\"\	\ ]\\+
setlocal spellfile=
setlocal spelllang=en
setlocal statusline=
setlocal suffixesadd=
setlocal noswapfile
setlocal synmaxcol=3000
if &syntax != 'qf'
setlocal syntax=qf
endif
setlocal tabstop=4
setlocal tags=
setlocal textwidth=0
setlocal thesaurus=
setlocal noundofile
setlocal winfixheight
setlocal nowinfixwidth
setlocal wrap
setlocal wrapmargin=0
wincmd w
2wincmd w
exe '1resize ' . ((&lines * 48 + 28) / 57)
exe 'vert 1resize ' . ((&columns * 27 + 106) / 212)
exe '2resize ' . ((&lines * 48 + 28) / 57)
exe 'vert 2resize ' . ((&columns * 95 + 106) / 212)
exe '3resize ' . ((&lines * 24 + 28) / 57)
exe 'vert 3resize ' . ((&columns * 88 + 106) / 212)
exe '4resize ' . ((&lines * 23 + 28) / 57)
exe 'vert 4resize ' . ((&columns * 88 + 106) / 212)
exe '5resize ' . ((&lines * 6 + 28) / 57)
tabnext 1
if exists('s:wipebuf')
  silent exe 'bwipe ' . s:wipebuf
endif
unlet! s:wipebuf
set winheight=1 winwidth=20 shortmess=filnxtToO
let s:sx = expand("<sfile>:p:r")."x.vim"
if file_readable(s:sx)
  exe "source " . fnameescape(s:sx)
endif
let &so = s:so_save | let &siso = s:siso_save
doautoall SessionLoadPost
unlet SessionLoad
" vim: set ft=vim :
