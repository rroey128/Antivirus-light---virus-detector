<h1>Antivirus light - virus detector</h1>


<h2>Description</h2>
This project is a C based virus detector - this program reads the signatures of the viruses from a designated signatures file and store these signatures in a dedicated linked list data structure. It then compares the virus signatures from the list to byte sequences from a suspected file, named in the command-line argument. If detected - the virus is then handled by manipulating directly the binary file using simple RET instruction in the 'problematic' function or section. (It is a simplified version of a modern anti-virus). The program itself runs on top of a very simple menu consists of simple anti-virus functionalities.
<br />


<h2>Languages and Utilities Used</h2>

- <b>C</b> 
- <b>Pointers and dynamically allocated structures and Valgrind utility</b>
- <b>Understanding 'data structures' in C</b>
- <b>Basic access to "binary" files, with application: simplified virus detection in executable files</b>

<h2>Environments Used </h2>
- <b>Linux Ubuntu 20.04 LTS</b> 

