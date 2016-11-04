" Configure for Vundle/YCM
" see
" http://www.ryanleaf.org/2015/10/22/getting-you-complete-me-ycm-to-work-on-fedora-22/

set nocompatible " Disable vi-compatibility
filetype off

set rtp+=~/.vim/bundle/Vundle.vim
call vundle#begin()
Plugin 'VundleVim/Vundle.vim'       " Package Manager
Plugin 'Valloric/YouCompleteMe'     " Omnicomplete automatically
Plugin 'powerline/powerline'
call vundle#end()

" Set some YMC options
" let g:ycm_filetype_whitelist = { '*': 1 }
let g:ycm_extra_conf_globlist = ['~/Software/phd/ngpt/*']

" Set the color-scheme (/usr/share/vim/vim74/colors)
colo slate

" Enable syntax highligting
syntax on

" Vertical line at 80-chars
set colorcolumn=80
highlight ColorColumn ctermbg=lightgrey guibg=lightgrey

" Show line numbers
set nu

" Indentation without tabs
set expandtab
set shiftwidth=4
set softtabstop=4

" Use intelligent indentation for C
set smartindent

" Highlight matching braces
set showmatch

" in makefiles, don't expand tabs to spaces, since actual tab characters are
" needed, and have indentation at 8 chars to be sure that all indents are
" tabs
" (despite the mappings later):
autocmd FileType make set noexpandtab shiftwidth=8 softtabstop=0

" mark keys for spanning through tabs
nnoremap <F3>  :tabnext<CR>
nnoremap <F2>  :tabprev<CR>

" <F5> enters timestamp
nnoremap <F5> "=strftime("%c")<CR>P

" When yanking, keep cursor at the end of yanked text
vnoremap y y`]

python from powerline.vim import setup as powerline_setup
python powerline_setup()
python del powerline_setup
let g:Powerline_symbols = 'fancy'
set laststatus=2
set t_Co=256 " Explicitly tell Vim that the terminal supports 256 colors
set encoding=utf-8 " Necessary to show Unicode glyphs
