#include "dbms.h"



int main(int argc, char *argv[])
{
    category *firstCategory = NULL, *lastCategory;  // initilazing for linked list
    struct product *firstProduct = NULL, *lastProduct;     // initilazing for linked list

    char buff[255], *command;
    int allComments[4] = {1, 2, 3, 4};      // 1-> CREATE, 2-> DELETE, 3-> SORT, 4->LIST
    int commentNumber;
    char *inputDir, *outpuDir;

           inputDir = argv[1];                   // getting input directory
           outpuDir = argv[2];                  // getting output directory

           FILE *fpInput, *fpOutput;
           fpInput=fopen(inputDir,"r");                       //open input file
           fpOutput = fopen(outpuDir,"w");                  //open input file

    if(fpInput == NULL){                             // if there is no such a file
        perror("Error opening input file.");
    }

    // read the input file and process commands
    else {
        while(fgets(buff,sizeof(buff),fpInput) != NULL){    //reading file line by line

            command = strtok(buff," ");    // tokenizing file

            // There are 4 commands and each commands has a commentNumber
            if ( strcmp(command,"CREATE") == 0 )
                 commentNumber = allComments[0];

            else if ( strcmp(command,"DELETE") == 0 )
                 commentNumber = allComments[1];

            else if ( strcmp(command,"SORT") == 0 )
                 commentNumber = allComments[2];

            else if ( strcmp(command,"LIST") == 0 )
                 commentNumber = allComments[3];

            // Main events occuring here
             switch(commentNumber){

                     case 1:                                                 // creating product or category or show all elemnts
                            command = strtok(NULL," ");

                            if ( strcmp(command,"CATEGORY") == 0 ){
                                char *trivial, *catName;

                                trivial = strtok(NULL," ");
                                catName = strtok(NULL,"\"");

                                createCategory(catName, &firstCategory, &lastCategory, fpOutput);
                            }

                            else if ( strcmp(command,"PRODUCT") == 0 ){
                                char *trivial, *productName, *catName, *pricePtr;
                                float price;

                                trivial = strtok(NULL," ");                            // trivial pointer holds unnecessary words
                                productName = strtok(NULL,"\"");

                                trivial = strtok(NULL," ");
                                catName = strtok(NULL,"\"");

                                trivial = strtok(NULL," ");
                                pricePtr = strtok(NULL," ");

                                price = strtof(pricePtr,NULL);        // casting price, string to float

                                createProduct(productName, catName, price, &firstProduct, &lastProduct, firstCategory, fpOutput);
                            }

                            else
                                createImage(firstCategory, firstProduct, fpOutput);

                            break;

                     case 2:{                                                              // deleting products if all conditions are okey

                            char *trivial, *productName, *catName;

                            // TOKEN COMMENT
                            trivial = strtok(NULL," ");     // PRODUCT
                            trivial = strtok(NULL," ");     // WHERE
                            trivial = strtok(NULL," ");     // NAME
                            trivial = strtok(NULL," ");     // IS

                            productName = strtok(NULL,"\"");    // product name

                            trivial = strtok(NULL," ");     // AND
                            trivial = strtok(NULL," ");     // CATEGORY
                            trivial = strtok(NULL," ");     // IS

                            catName = strtok(NULL,"\"");        // category name

                            deleteProduct( productName, catName, &firstProduct, firstCategory, fpOutput);

                            break;
                     }

                     case 3:{                                                                 // sort products according to specific comments

                            char *trivial, *sortBy, *sortType;

                            // TOKEN COMMENT
                            trivial = strtok(NULL," ");     // PRODUCT
                            trivial = strtok(NULL," ");     // BY

                            sortBy = strtok(NULL," ");     // PRICE or NAME
                            sortType = strtok(NULL,"\n");     // ASC or DESC



                            if( strcmp(sortBy,"PRICE") == 0 ){

                                if ( strcmp(sortType,"ASC") == 0 )
                                    sortByPriceAsc(&firstProduct, &lastProduct, fpOutput);
                                else
                                    sortByPriceDesc(&firstProduct, &lastProduct, fpOutput);
                            }

                            else {

                                if ( strcmp(sortType,"ASC") == 0 )
                                    sortByNameAsc(&firstProduct, &lastProduct, fpOutput);
                                else
                                    sortByNameDesc(&firstProduct, &lastProduct, fpOutput);
                            }

                     }
                            break;

                     case 4:                                                                  // list products according to specific comments
                            command = strtok(NULL," ");

                            if ( strcmp(command,"CATEGORY") == 0 )   //list all categories
                                listCategoryAll(firstCategory, fpOutput);

                            else if ( strcmp(command,"PRODUCT") == 0 ){

                                command = strtok(NULL," ");

                                if ( strcmp(command,"WHERE") == 0 ){

                                    command = strtok(NULL," ");

                                    if ( strcmp(command,"NAME") == 0 ){

                                        char *trivial = strtok(NULL," ");
                                        char *productName = strtok(NULL,"\"");

                                        listProductByName(productName, firstProduct, fpOutput);
                                    }

                                    else if ( strcmp(command,"PRICE") == 0 ){

                                        char *pricePtr ,*command;

                                        command = strtok(NULL," ");

                                            if ( strcmp(command,"IS") == 0){             // LIST PRODUCT WHERE PRICE IS ...

                                                float price;

                                                pricePtr = strtok(NULL,"\n");
                                                price = strtof(pricePtr,NULL);      // casting string to float

                                                listProductByPrice(price, firstProduct, fpOutput);
                                            }

                                            else {                                      // LIST PRODUCT WHERE PRICE BETWEEN

                                                float price1, price2;

                                                pricePtr = strtok(NULL ," ");
                                                price1 = strtof(pricePtr,NULL);

                                                pricePtr = strtok(NULL ,"\n");
                                                price2 = strtof(pricePtr,NULL);

                                                listProductBetweenPrices(price1, price2, firstProduct, fpOutput);
                                            }

                                    }

                                    else if ( strcmp(command,"CATEGORY") == 0 ){

                                        char *trivial = strtok(NULL," ");
                                        char *catName = strtok(NULL,"\"");

                                        listProductByCategory(catName, firstProduct, firstCategory, fpOutput);
                                    }

                                }

                                else                                                          //list all products
                                    listProductAll(firstProduct, fpOutput);
//
                             }// end else if  command = product

                            break;

                     default: break;
             }

        }//end of while

         // close file when process end
        fclose(fpInput);
        fclose(fpOutput);

    }

    return 0;
}// end main

    void createCategory( char *catName, category **firstCategory, category **lastCategory, FILE *fp){

        category *newCategory;
        static int cat_id = 1;


            newCategory=(category *)malloc(sizeof(category));

            strcpy(newCategory->categoryName,catName);
            newCategory->cat_id = cat_id;
            newCategory->nextCategory = NULL;

            if ( *firstCategory == NULL ){               // initilazing firstCategory

                *firstCategory = newCategory;
                *lastCategory = newCategory;

                fprintf(fp,"Command:\nCREATE CATEGORY NAME \"%s\"\nResult:\nSUCCESSFUL\n\n", catName);

                cat_id ++;

            }

            else {

                category *controlCategoryName = *firstCategory;
                int noReplicate;

                while (strcmp(controlCategoryName->categoryName,catName) != 0 ) {         // control replication -> search from begin category to end

                    controlCategoryName = controlCategoryName->nextCategory;

                    if(controlCategoryName == NULL){          // if there is no replicate
                        noReplicate = 1;
                        break;
                    }

                }

                if(noReplicate == 1){                // if there is no replication

                    (*lastCategory)->nextCategory = newCategory;
                    (*lastCategory) = newCategory;
                    fprintf(fp,"Command:\nCREATE CATEGORY NAME \"%s\"\nResult:\nSUCCESSFUL\n\n", catName);

                    cat_id ++;

                }

                else                                    // there is replicate
                    fprintf(fp,"Command:\nCREATE CATEGORY NAME \"%s\"\nResult:\nFAILED\n\n", catName);

            }


        return 0;
    }//end createCategory

    void createProduct( char *productName, char *catName, float price, product **firstProduct, product **lastProduct, category *firstCategory, FILE *fp){

            category *controlCategory = firstCategory;
            product *newProduct, *controlProduct = *firstProduct;
            static int product_id = 1;
            int allowCreate = 0;

            // first of all create newProduct then add linked list if all conditions are okey
            newProduct=(product *)malloc(sizeof(product));

            strcpy(newProduct->productName,productName);
            newProduct->product_id = product_id;
            newProduct->productPrice = price;
            strcpy(newProduct->productCatName,catName);
            newProduct->nextProduct = NULL;



            while ( controlCategory != NULL ) {         // is category registered ??

                    if(strcmp(controlCategory->categoryName,catName) == 0){  // is that category available

                        allowCreate = 1;
                        break;
                    }

                    controlCategory = controlCategory->nextCategory;

            }

            // controls replicate products in the same category
            while ( controlProduct != NULL ) {

                    if(strcmp(controlProduct->productName,productName) == 0){  // same name available

                        if ( strcmp(controlProduct->productCatName, catName) == 0 ){    // same name in same category

                            allowCreate = 0;
                            break;
                        }

                    }

                    controlProduct = controlProduct->nextProduct;

            }

            // if all conditions is okey add product
            if(allowCreate == 1){

                    if ( *firstProduct == NULL ){               // initilazing firstProduct



                        *firstProduct = newProduct;
                        *lastProduct = newProduct;

                        fprintf(fp,"Command:\nCREATE PRODUCT NAME \"%s\" CATEGORY \"%s\" PRICE %.2f\nResult:\nSUCCESSFUL\n\n",productName,catName,price);

                    }

                    else{



                        (*lastProduct)->nextProduct = newProduct;
                        (*lastProduct) = newProduct;

                        fprintf(fp,"Command:\nCREATE PRODUCT NAME \"%s\" CATEGORY \"%s\" PRICE %.2f\nResult:\nSUCCESSFUL\n\n",productName,catName,price);

                    }

                    product_id ++;

            }

            else           // product not allowed for creating
                fprintf(fp,"Command:\nCREATE PRODUCT NAME \"%s\" CATEGORY \"%s\" PRICE %.2f\nResult:\nFAILED\n\n",productName,catName,price);

    }//end createProduct

    void createImage(category *firstCategory,product *firstProduct, FILE *fp){

        category *xCategory = firstCategory;
        product *xProduct = firstProduct;

        fprintf(fp,"Command:\nCREATE IMAGE\nResult:\n");

        if ( xCategory == NULL ){             // if there is no category
            fprintf(fp,"EMPTY\n\n");
        }

        else {

                while ( xCategory != NULL ){  // print categories respectively

                    if(xCategory->nextCategory == NULL && xProduct == NULL){            // puts double \n for last line of create image command's output
                        fprintf(fp,"CATEGORY NAME \"%s\"\n\n",xCategory->categoryName);
                        break;
                    }
                    else
                        fprintf(fp,"CATEGORY NAME \"%s\"\n",xCategory->categoryName);

                    xCategory = xCategory->nextCategory;
                }

                if ( xProduct != NULL ){             // if there is any product

                     while ( xProduct != NULL ){  // print products respectively

                        if( xProduct->nextProduct == NULL){            // puts double \n for last line of create image command's output
                            fprintf(fp,"PRODUCT NAME \"%s\" CATEGORY \"%s\" PRICE %.2f\n\n", xProduct->productName, xProduct->productCatName, xProduct->productPrice);
                            break;
                        }
                        else
                            fprintf(fp,"PRODUCT NAME \"%s\" CATEGORY \"%s\" PRICE %.2f\n", xProduct->productName, xProduct->productCatName, xProduct->productPrice);

                        xProduct = xProduct->nextProduct;
                    }

                }

        }// end else

    }//createImage

    void listCategoryAll(category *firstCategory, FILE *fp){

        category *xCategory = firstCategory;

        fprintf(fp,"Command:\nLIST CATEGORY ALL\nResult:\n");

        if ( xCategory == NULL)
            fprintf(fp,"EMPTY\n\n");

        else {

            while ( xCategory != NULL ){  // print categories respectively

                    if(xCategory->nextCategory == NULL ){            // puts double \n for last line of LIST CATEGORY command's output
                        fprintf(fp,"CATEGORY NAME \"%s\"\n\n",xCategory->categoryName);
                        break;
                    }
                    else
                        fprintf(fp,"CATEGORY NAME \"%s\"\n",xCategory->categoryName);

                    xCategory = xCategory->nextCategory;
            }

        }

    }//end listCategoryAll

    void listProductAll(product *firstProduct, FILE *fp){

        product *xProduct = firstProduct;

        fprintf(fp,"Command:\nLIST PRODUCT ALL\nResult:\n");

        if ( xProduct == NULL)
            fprintf(fp,"EMPTY\n\n");

        else {

            while ( xProduct != NULL ){  // print categories respectively

                    if(xProduct->nextProduct == NULL ){            // puts double \n for last line of LIST CATEGORY command's output
                        fprintf(fp,"PRODUCT NAME \"%s\" CATEGORY \"%s\" PRICE %.2f\n\n", xProduct->productName, xProduct->productCatName, xProduct->productPrice);
                        break;
                    }
                    else
                        fprintf(fp,"PRODUCT NAME \"%s\" CATEGORY \"%s\" PRICE %.2f\n", xProduct->productName, xProduct->productCatName, xProduct->productPrice);

                    xProduct = xProduct->nextProduct;
            }

        }

    }//end listProductAll

    void listProductByName(char *productName, product *firstProduct, FILE *fp){

        product *xProduct = firstProduct;
        int controlProductName = 0;                                  // if there is no such a product controlProductName stay 0;

        fprintf(fp,"Command:\nLIST PRODUCT WHERE NAME IS \"%s\"\nResult:\n",productName);

        if(xProduct == NULL)
            fprintf(fp,"EMPTY\n\n");

        else{

            while ( xProduct != NULL ){

                    if( strcmp(xProduct->productName,productName) == 0 ){            // puts double \n for last line of LIST PRODUCT WHERE NAME IS command's output

                        controlProductName++;
                        fprintf(fp,"PRODUCT NAME \"%s\" CATEGORY \"%s\" PRICE %.2f\n", xProduct->productName, xProduct->productCatName, xProduct->productPrice);
                    }
                    xProduct = xProduct->nextProduct;
            }

            if ( !controlProductName )
                fprintf(fp,"EMPTY\n\n");
            else
                fprintf(fp,"\n");    // put \n for last line of LIST PRODUCT WHERE NAME IS command's output
        }

    }//end listProductByName

    void listProductByPrice(float price, product *firstProduct, FILE *fp){

        product *xProduct = firstProduct;
        int controlProductPrice = 0;                                  // if there is no such a product controlProductName stay 0;

        fprintf(fp,"Command:\nLIST PRODUCT WHERE PRICE IS %.2f\nResult:\n",price);

        if(xProduct == NULL)                             // if there is no product yet
            fprintf(fp,"EMPTY\n\n");

        else{

            while ( xProduct != NULL ){

                    if( xProduct->productPrice == price ){            // puts double \n for last line of LIST PRODUCT WHERE NAME IS command's output

                        controlProductPrice++;
                        fprintf(fp,"PRODUCT NAME \"%s\" CATEGORY \"%s\" PRICE %.2f\n", xProduct->productName, xProduct->productCatName, xProduct->productPrice);
                    }
                    xProduct = xProduct->nextProduct;
            }

            if ( !controlProductPrice )
                fprintf(fp,"EMPTY\n\n");
            else
                fprintf(fp,"\n");    // put \n for last line of LIST PRODUCT WHERE NAME IS command's output
        }

    }//end listProductByPrice

    void listProductByCategory(char *catName, product *firstProduct, category *firstCategory, FILE *fp){

        product *xProduct = firstProduct;
        category *xCategory = firstCategory;
        int controlCatName = 0;

        fprintf(fp,"Command:\nLIST PRODUCT WHERE CATEGORY IS \"%s\"\nResult:\n",catName);

        if( xCategory == NULL )             // there is no category
            fprintf(fp,"EMPTY\n\n");

        else if ( xProduct == NULL )        // there is no product
            fprintf(fp,"EMPTY\n\n");

        else {

                while ( xProduct != NULL ){

                    if( strcmp(xProduct->productCatName,catName) == 0 ){            // puts double \n for last line of LIST PRODUCT WHERE CATEGORY IS command's output

                        controlCatName++;
                        fprintf(fp,"PRODUCT NAME \"%s\" CATEGORY \"%s\" PRICE %.2f\n", xProduct->productName, catName, xProduct->productPrice);
                    }

                    xProduct = xProduct->nextProduct;
                }// end while

                if ( !controlCatName )
                    fprintf(fp,"EMPTY\n\n");
                else
                    fprintf(fp,"\n");    // put \n for last line of LIST PRODUCT WHERE CATEGORY IS command's output
        }

    }// end listProductByPrice

    void listProductBetweenPrices(float price1, float price2, product *firstProduct, FILE *fp){

        product *xProduct = firstProduct;
        float smallPrice = price2 , biggerPrice = price1;
        int controlProduct = 0;

        if( price1 < price2){                  // find bigger number
            biggerPrice = price2;
            smallPrice = price1;
        }

        fprintf(fp,"Command:\nLIST PRODUCT WHERE PRICE BETWEEN %.2f %.2f\nResult:\n", price1, price2);

        if( firstProduct == NULL)
            fprintf(fp,"EMPTY\n\n");

        else {

            while ( xProduct != NULL ){

                if( smallPrice <= xProduct->productPrice && xProduct->productPrice <= biggerPrice ){           // if price between smallprice and biggerprice

                    controlProduct++;
                    fprintf(fp,"PRODUCT NAME \"%s\" CATEGORY \"%s\" PRICE %.2f\n", xProduct->productName, xProduct->productCatName, xProduct->productPrice);
                }
                 xProduct = xProduct->nextProduct;

            }//end while

            if ( !controlProduct )
                fprintf(fp,"EMPTY\n\n");
            else
                fprintf(fp,"\n");    // put \n for last line of LIST PRODUCT WHERE BETWEEN PRICE IS command's output

        }

    }//end listProductBetweenPrices

    void deleteProduct(char *productName, char *catName, product **firstProduct, category *firstCategory, FILE *fp){
        product *afterProduct, *xProduct = *firstProduct;
        category *xCategory = firstCategory;
        int controlProduct = 0;

        fprintf(fp,"Command:\nDELETE PRODUCT WHERE NAME IS \"%s\" AND CATEGORY IS \"%s\"\nResult:\n", productName, catName);

        if( xCategory == NULL )             // there is no category
            fprintf(fp,"FAILED\n\n");
        else if ( xProduct == NULL )        // there is no product
            fprintf(fp,"FAILED\n\n");
        else if ( strcmp(xProduct->productName,productName) == 0 && strcmp(xProduct->productCatName,catName) == 0 ){    // for deleting first product
            *firstProduct = (*firstProduct)->nextProduct;         // 2.product is now first product
            xProduct->nextProduct = NULL;                      // remove first product from linked list

            fprintf(fp,"SUCCESSFUL\n\n");
        }
        else {
                while ( xProduct->nextProduct != NULL ){
                    afterProduct = xProduct->nextProduct;
                    if ( strcmp(afterProduct->productName,productName) == 0 && strcmp(afterProduct->productCatName,catName) == 0){
                        xProduct->nextProduct = afterProduct->nextProduct;
                        controlProduct++;

                        fprintf(fp,"SUCCESSFUL\n");
                        break;
                    }
                    xProduct = xProduct->nextProduct;
                }// end while

                if ( !controlProduct )
                    fprintf(fp,"FAILED\n\n");
                else
                    fprintf(fp,"\n");    // put \n for last line of DELETE PRODUCT command's output
        }
    }// end listProductBetweenPrices

    void sortByPriceDesc (product **firstProduct, product **lastProduct, FILE *fp){
        int numberOfProduct = 0, i=0;
        product *beforeProduct, *currentProduct, *afterProduct, *tempProduct , *counter, *secondProduct;

        fprintf(fp,"Command:\nSORT PRODUCT BY PRICE DESC\nResult:\nSUCCESSFUL\n\n");

        if ( *firstProduct != NULL){
            secondProduct = (*firstProduct)->nextProduct;                        //  second product
            if ( secondProduct != NULL ){             // if there are more than one product
                counter = *firstProduct;
                // finds number of products
                while ( counter != NULL ){
                    numberOfProduct++;
                    counter = counter->nextProduct;
                }

                for (i=0; i<numberOfProduct; i++){                // compare products, number of products times
                    secondProduct = (*firstProduct)->nextProduct;
                    if ((*firstProduct)->productPrice < secondProduct->productPrice ){       // changing first two products place
                            tempProduct = secondProduct->nextProduct;
                            (*firstProduct)->nextProduct = tempProduct;
                            secondProduct->nextProduct = *firstProduct;
                            (*firstProduct) = secondProduct;
                            secondProduct = (*firstProduct)->nextProduct;
                    }
                    beforeProduct = *firstProduct;
                    currentProduct = secondProduct;
                    afterProduct = currentProduct->nextProduct;

                    while ( afterProduct != NULL ){;
                        if ( currentProduct->productPrice < afterProduct->productPrice){          // changing other products place
                            tempProduct = afterProduct->nextProduct;
                            currentProduct->nextProduct = tempProduct;
                            afterProduct->nextProduct = currentProduct;
                            beforeProduct->nextProduct = afterProduct;
                        }
                        beforeProduct = beforeProduct->nextProduct;
                        currentProduct = beforeProduct->nextProduct;
                        *lastProduct = currentProduct;
                        afterProduct = currentProduct->nextProduct;
                    }// end while
                }// end for
            }// end if
        }// end if
    }// end sortByPriceDesc

    void sortByPriceAsc (product **firstProduct, product **lastProduct, FILE *fp){
        int numberOfProduct = 0, i=0;
        product *beforeProduct, *currentProduct, *afterProduct, *tempProduct , *counter, *secondProduct;
        fprintf(fp,"Command:\nSORT PRODUCT BY PRICE ASC\nResult:\nSUCCESSFUL\n\n");

        if ( *firstProduct != NULL){
            secondProduct = (*firstProduct)->nextProduct;                        //  second product
            if ( secondProduct != NULL ){             // if there are more than one product
                counter = *firstProduct;
                // finds number of products
                while ( counter != NULL ){
                    numberOfProduct++;
                    counter = counter->nextProduct;
                }

                for (i=0; i<numberOfProduct; i++){                // compare products, number of products times
                    secondProduct = (*firstProduct)->nextProduct;
                    if ((*firstProduct)->productPrice > secondProduct->productPrice ){       // changing first two products place
                            tempProduct = secondProduct->nextProduct;
                            (*firstProduct)->nextProduct = tempProduct;
                            secondProduct->nextProduct = *firstProduct;
                            (*firstProduct) = secondProduct;
                            secondProduct = (*firstProduct)->nextProduct;
                    }

                    beforeProduct = *firstProduct;
                    currentProduct = secondProduct;
                    afterProduct = currentProduct->nextProduct;

                    while ( afterProduct != NULL ){;
                        if ( currentProduct->productPrice > afterProduct->productPrice){          // changing other products place
                            tempProduct = afterProduct->nextProduct;
                            currentProduct->nextProduct = tempProduct;
                            afterProduct->nextProduct = currentProduct;
                            beforeProduct->nextProduct = afterProduct;
                        }

                        beforeProduct = beforeProduct->nextProduct;
                        currentProduct = beforeProduct->nextProduct;
                        *lastProduct = currentProduct;
                        afterProduct = currentProduct->nextProduct
                    }// end while
                }// end for
            }// end if
        }// end if
    }// end sortByPriceAsc

    void sortByNameDesc (product **firstProduct, product **lastProduct, FILE *fp){
        int numberOfProduct = 0, i=0;
        product *beforeProduct, *currentProduct, *afterProduct, *tempProduct , *counter, *secondProduct;

        fprintf(fp,"Command:\nSORT PRODUCT BY NAME DESC\nResult:\nSUCCESSFUL\n\n");

        if ( *firstProduct != NULL){
            secondProduct = (*firstProduct)->nextProduct;                        //  second product
            if ( secondProduct != NULL ){             // if there are more than one product
                counter = *firstProduct;
                while ( counter != NULL ){
                    numberOfProduct++;                      // finds number of products
                    counter = counter->nextProduct;
                }

                for (i=0; i<numberOfProduct; i++){                // compare products, number of products times
                    secondProduct = (*firstProduct)->nextProduct;
                    if ( strcmp((*firstProduct)->productName,secondProduct->productName) < 0 ){       // changing first two products place
                            tempProduct = secondProduct->nextProduct;
                            (*firstProduct)->nextProduct = tempProduct;
                            secondProduct->nextProduct = *firstProduct;
                            (*firstProduct) = secondProduct;
                            secondProduct = (*firstProduct)->nextProduct;
                    }

                    beforeProduct = *firstProduct;
                    currentProduct = secondProduct;
                    afterProduct = currentProduct->nextProduct;

                    while ( afterProduct != NULL ){
                        if ( strcmp(currentProduct->productName,afterProduct->productName) < 0 ){          // changing other products place
                            tempProduct = afterProduct->nextProduct;
                            currentProduct->nextProduct = tempProduct;
                            afterProduct->nextProduct = currentProduct;
                            beforeProduct->nextProduct = afterProduct;
                        }

                        beforeProduct = beforeProduct->nextProduct;
                        currentProduct = beforeProduct->nextProduct;
                        *lastProduct = currentProduct;
                        afterProduct = currentProduct->nextProduct;
                    }// end while
                }// end for
            }// end if
        }// end if
    }// end sortByNameDesc
    void sortByNameAsc (product **firstProduct, product **lastProduct, FILE *fp){
        int numberOfProduct = 0, i=0;
        product *beforeProduct, *currentProduct, *afterProduct, *tempProduct , *counter, *secondProduct;

        fprintf(fp,"Command:\nSORT PRODUCT BY NAME ASC\nResult:\nSUCCESSFUL\n\n");

        if ( *firstProduct != NULL){
            secondProduct = (*firstProduct)->nextProduct;                        //  second product
            if ( secondProduct != NULL ){             // if there are more than one product
                counter = *firstProduct;
                while ( counter != NULL ){
                    numberOfProduct++;                      // finds number of products
                    counter = counter->nextProduct;
                }

                for (i=0; i<numberOfProduct; i++){                // compare products number of products times
                    secondProduct = (*firstProduct)->nextProduct;
                    if ( strcmp((*firstProduct)->productName,secondProduct->productName) > 0 ){       // changing first two products place
                            tempProduct = secondProduct->nextProduct;
                            (*firstProduct)->nextProduct = tempProduct;
                            secondProduct->nextProduct = *firstProduct;
                            (*firstProduct) = secondProduct;
                            secondProduct = (*firstProduct)->nextProduct;
                    }
                    beforeProduct = *firstProduct;
                    currentProduct = secondProduct;
                    afterProduct = currentProduct->nextProduct;

                    while ( afterProduct != NULL ){;
                        if ( strcmp(currentProduct->productName,afterProduct->productName) > 0 ){          // changing other products place
                            tempProduct = afterProduct->nextProduct;
                            currentProduct->nextProduct = tempProduct;
                            afterProduct->nextProduct = currentProduct;
                            beforeProduct->nextProduct = afterProduct;
                        }
                        beforeProduct = beforeProduct->nextProduct;
                        currentProduct = beforeProduct->nextProduct;
                        *lastProduct = currentProduct;
                        afterProduct = currentProduct->nextProduct;
                    }// end while
                }// end for
            }// end if
        }// end if
    }// end sortByNameAsc


