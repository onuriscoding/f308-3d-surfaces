%%%% Template for ULB report %%%%
	  % By Boris Coquelet %
	  
%Dans le cadre du poste aide à la réussite au Bureau des étudiants de Polytechnique%
%2019
% Version 1.0

\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{ULBreport}
\LoadClass[a4paper,12pt,openany]{report}


%%%%Structure%%%%

\RequirePackage[utf8]{inputenc}
\RequirePackage[T1]{fontenc}
%Language
\RequirePackage{csquotes}
\RequirePackage[french]{babel}
%silence the annoying warning
\frenchsetup{SuppressWarning}

\RequirePackage{tabularx}
\RequirePackage[top=2cm,bottom=2cm,left=2cm,right=2cm]{geometry}
\RequirePackage{hyperref}
\RequirePackage{enumitem}
\RequirePackage{xkeyval}
\RequirePackage{ifthen}

%%%%package tikz%%%%

\RequirePackage{tikz}
\usetikzlibrary{mindmap}
\RequirePackage{chemfig}
\RequirePackage{pgfplots}
\pgfplotsset{compat=newest}

%%%%Forme%%%%

%Chapter form
\usepackage{titlesec, blindtext, color}

%Picture package
\RequirePackage{graphicx}
\RequirePackage{textcomp}
\RequirePackage{eurosym}
\RequirePackage{mwe}
\RequirePackage{listings}
\RequirePackage{etoolbox}
\RequirePackage{setspace}
\RequirePackage{caption}
\RequirePackage{colortbl}
\RequirePackage{framed}
\RequirePackage{xcolor}
\RequirePackage{fourier}
\RequirePackage[pages=some]{background}

%%%%Liens%%%

\usepackage{hyperref}
\hypersetup{
	colorlinks=true,
	linkcolor=blue,
	filecolor=magenta,
	urlcolor=cyan,
	citecolor=lightgray,
	pdftitle={Overleaf Example},
	pdfpagemode=FullScreen,
}

\usepackage[numbers]{natbib}

%%%%Math%%%%

\RequirePackage{mathtools}
\RequirePackage{amsmath}
\RequirePackage{amsfonts}
\RequirePackage{amssymb}
\RequirePackage{mathrsfs}
\usetikzlibrary{arrows,matrix,positioning}
\usetikzlibrary{3d,calc}
\RequirePackage[makeroom]{cancel}
\RequirePackage{esint}

%%%%Annexe%%%%

% Table of contents
\RequirePackage[toc,page]{appendix}
\linespread{1.5}
\renewcommand{\appendixtocname}{Annexes}
\renewcommand{\appendixpagename}{Annexes}

% Revisions
\usepackage{tabularray}
\DefTblrTemplate{contfoot-text}{default}{}
\DefTblrTemplate{conthead-text}{default}{}
\DefTblrTemplate{caption}{default}{}
\DefTblrTemplate{conthead}{default}{}
\DefTblrTemplate{capcont}{default}{}

%%%%Couleurs%%%%

\definecolor{apricot}{rgb}{0.98, 0.81, 0.69}
\definecolor{bleu}{rgb}{0.19, 0.55, 0.91}
\definecolor{rouge}{rgb}{0.77, 0.12, 0.23}
\definecolor{vert}{rgb}{0.13, 0.55, 0.13}
\definecolor{antiquefuchsia}{rgb}{0.57, 0.36, 0.51}
\colorlet{shadecolor}{cyan!30!white}
\colorlet{framecolor}{blue!50!black}

%%%%Custom command%%%%

\newcommand{\rouge}[1]{\textcolor{rouge}{#1}}
\newcommand{\bleu}[1]{\textcolor{bleu}{#1}}
\newcommand{\verts}[1]{\textcolor{vert}{#1}}

\makeatletter

%%%Headers%%%

\usepackage{lastpage}
\usepackage{fancyhdr}

\fancypagestyle{fancy}{%
	\fancyhf{}
	\fancyhead[RO,LE]{\textcolor{antiquefuchsia}{\leftmark}}
	\fancyfoot[C]{\rouge{\thepage}\,/\,\bleu{\pageref*{LastPage}}}%
	\let\oldheadrule\headrule%
	\renewcommand{\headrule}{\color{antiquefuchsia}\oldheadrule}%
	\renewcommand{\headrulewidth}{0.4pt}%
	\renewcommand{\footrulewidth}{0pt}%
}

% Redefine plain style, issued by new chapters
\fancypagestyle{plain}{%
	\fancyhf{}
	\fancyfoot[C]{\rouge{\thepage}\,/\,\bleu{\getpagerefnumber{LastPage}}}%
	\renewcommand{\headrulewidth}{0pt}%
	\renewcommand{\footrulewidth}{0pt}%
}

%%%Title page%%%

%Keys for title page
\define@cmdkey [PRE] {fam} {title}{}
\define@cmdkey [PRE] {fam} {studies}{}
\define@cmdkey [PRE] {fam} {course}{}
\define@cmdkey [PRE] {fam} {author}{}
\define@cmdkey [PRE] {fam} {teacher}{}
\define@cmdkey [PRE] {fam} {date}{}
\define@cmdkey [PRE] {fam} {logo}{}
\define@cmdkey [PRE] {fam} {logoo}{}
\define@cmdkey [PRE] {fam} {manyAuthor}[1]{}
%Set default value for the keys (used if not specified)
\presetkeys         [PRE] {fam} {title=titre,
								 studies=,
								 course=,
								 author=Auteurs,
								 teacher=,
								 date=,
								 logo=7,
								 logoo=6,
                                 manyAuthor=8,}{}
\tikzset{vertical custom shading/.code={%
 \pgfmathsetmacro\tikz@vcs@middle{#1}
 \pgfmathsetmacro\tikz@vcs@bottom{\tikz@vcs@middle/2}
 \pgfmathsetmacro\tikz@vcs@top{(100-\tikz@vcs@middle)/2+\tikz@vcs@middle}
\pgfdeclareverticalshading[tikz@axis@top,tikz@axis@middle,tikz@axis@bottom]{newaxis}{100bp}{%
  color(0bp)=(tikz@axis@bottom);
  color(\tikz@vcs@bottom bp)=(tikz@axis@bottom);
  color(\tikz@vcs@middle bp)=(tikz@axis@middle);
  color(\tikz@vcs@top bp)=(tikz@axis@top);
  color(100bp)=(tikz@axis@top)}
  \pgfkeysalso{/tikz/shading=newaxis}
  }
}

% Title command
\newcommand*{\titleULB}[1]{\setkeys[PRE]{fam}{#1}

	\begingroup
	\BgThispage
	% Replace logo with nothing if not specified
		\begin{center}
	\ifthenelse{\equal{\cmdPRE@fam@logo}{7}}{}{\includegraphics[height=3cm,width=10cm,keepaspectratio]{\cmdPRE@fam@logo}\\}
	{\cmdPRE@fam@studies}
	\end{center}
	\begin{center}
	\vspace*{\baselineskip}
	{\bfseries \cmdPRE@fam@course}\\[\baselineskip]
	\rule{\textwidth}{1.6pt}\vspace*{-\baselineskip}\vspace*{2pt}
	\rule{\textwidth}{0.4pt}\\[\baselineskip]
	{\Huge \cmdPRE@fam@title}\\[\baselineskip]
	\rule{\textwidth}{0.4pt}\vspace*{-\baselineskip}\vspace{3.2pt}
	\rule{\textwidth}{1.6pt}\\[\baselineskip]
	\ifthenelse{\equal{\cmdPRE@fam@manyAuthor}{8}}{
	{\Large \cmdPRE@fam@author}\\[\baselineskip]
	{\bfseries \cmdPRE@fam@teacher}}{
	    \begin{minipage}{0.4\textwidth}
	         \large
	        \cmdPRE@fam@author
	        
	    \end{minipage}
	    \begin{minipage}{0.4\textwidth}
	        \begin{center} \large
	        \cmdPRE@fam@teacher
	        \end{center}
	    \end{minipage}
	}
	\vfill
	{\cmdPRE@fam@date}
	\end{center}
	\vspace*{3\baselineskip}
	\pagenumbering{gobble}
	\newpage
    \newpage
    \pagenumbering{arabic}
\endgroup}

% Fix on chapter overful hbox

\usepackage{titlesec}

\titleformat{\chapter}[hang]{\Huge\bfseries}{}{0pt}{}

% Définir le style des pages avec chapitres.
\makeatletter

\makeatother


%Boxed environement

%Command to put the seal of ULB in Background of the first page
\newcommand{\sceau}[1]{\backgroundsetup{
scale=1,
color=black,
opacity=0.25,
angle=0,
position={17cm,-25cm},
contents={%
  \includegraphics[height=35cm,width=35cm,keepaspectratio]{#1}
  }%
}}

%Add small heading in the left corner of the page.
\pagestyle{empty}

%Copyright symbol
\let\oldtextcopyright\textcopyright
\renewcommand{\textcopyright}{\textsuperscript{\oldtextcopyright}}%
