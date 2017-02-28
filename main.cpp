int file;
 file= open ("aa.jpg", O_RDWR , 0600);
    int caracter,tam;
    unsigned char byte[1];
        while( tam=read(file, &byte, 1)>0)
          cout << byte;
            }

