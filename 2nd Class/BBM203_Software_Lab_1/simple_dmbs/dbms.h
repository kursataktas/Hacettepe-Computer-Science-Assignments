#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#ifndef DBMS_H_INCLUDED
#define DBMS_H_INCLUDED

typedef struct categories {

int cat_id;
char categoryName[30];
struct category *nextCategory;
}category;

typedef struct products{

int product_id;
float productPrice;
char productName[101];
char productCatName[30];
struct product *nextProduct;
}product;

void createCategory( char *catName, category **firstCategory, category **lastCategory, FILE *fp);
void createProduct( char *productName, char *catName, float price, product **firstProduct, product **lastProduct, category *firstCategory, FILE *fp);
void createImage(category *firstCategory, product *firstProduct, FILE *fp);
void listCategoryAll(category *firstCategory, FILE *fp);
void listProductAll(product *firstProduct, FILE *fp);
void listProductByName(char *productName, product *firstProduct, FILE *fp);
void listProductByPrice(float price, product *firstProduct, FILE *fp);
void listProductByCategory(char *catName, product *firstProduct, category *firstCategory, FILE *fp);
void listProductBetweenPrices(float price1, float price2, product *firstProduct, FILE *fp);
void deleteProduct(char *productName, char *catName, product **firstProduct, category *firstCategory, FILE *fp);
void sortByPriceDesc (product **firstProduct, product **lastProduct, FILE *fp);
void sortByPriceAsc (product **firstProduct, product **lastProduct,FILE *fp);
void sortByNameDesc (product **firstProduct, product **lastProduct,FILE *fp);
void sortByNameAsc (product **firstProduct, product **lastProduct,FILE *fp);
#endif // DBMS_H_INCLUDED
