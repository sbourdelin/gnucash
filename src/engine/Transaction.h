/********************************************************************\
 * Transaction.h -- defines transaction for xacc (X-Accountant)     *
 * Copyright (C) 1997 Robin D. Clark                                *
 * Copyright (C) 1997, 1998 Linas Vepstas                           *
 *                                                                  *
 * This program is free software; you can redistribute it and/or    *
 * modify it under the terms of the GNU General Public License as   *
 * published by the Free Software Foundation; either version 2 of   *
 * the License, or (at your option) any later version.              *
 *                                                                  *
 * This program is distributed in the hope that it will be useful,  *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of   *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the    *
 * GNU General Public License for more details.                     *
 *                                                                  *
 * You should have received a copy of the GNU General Public License*
 * along with this program; if not, write to the Free Software      *
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.        *
 *                                                                  *
 *   Author: Rob Clark                                              *
 * Internet: rclark@cs.hmc.edu                                      *
 *  Address: 609 8th Street                                         *
 *           Huntington Beach, CA 92648-4632                        *
\********************************************************************/

#ifndef __XACC_TRANSACTION_H__
#define __XACC_TRANSACTION_H__

#include "config.h"
#include "date.h"   /* for Date */

/* Values for the reconciled field in Transaction: */
#define CREC 'c'              /* The transaction has been cleared        */
#define YREC 'y'              /* The transaction has been reconciled     */
#define FREC 'f'              /* frozen into accounting period           */
#define NREC 'n'              /* not reconciled or cleared               */

/** STRUCTS *********************************************************/
/* The debit & credit pointers are used to implement a double-entry 
 * accounting system.  Basically, the idea with double entry is that
 * there is always an account that is debited, and another that is
 * credited.  These two pointers identify the two accounts. 
 */

/* A split transaction is one which shows up as a credit (or debit) in
 * one account, and peices of it show up as debits (or credits) in other
 * accounts.  Thus, a single credit-card transaction might be split
 * between "dining", "tips" and "taxes" categories.
 */

typedef struct _split        Split;
typedef struct _transaction  Transaction;


/** PROTOTYPES ******************************************************/
/*
 * The xaccTransSetDate() method will modify the date of the 
 *    transaction.  It will also make sure that the transaction
 *    is stored in proper date order in the accounts.
 */

Split       * xaccMallocSplit (void);
void          xaccInitSplit   (Split *);    /* clears a split struct */
void          xaccFreeSplit   (Split *);    /* frees memory */
int           xaccCountSplits (Split **sarray);

Transaction * xaccMallocTransaction (void);       /* mallocs and inits */
void          xaccInitTransaction (Transaction *);/* clears a trans struct */

void          xaccTransSetDate (Transaction *, int day, int mon, int year);

/* set the transaction date to the current system time. */
void          xaccTransSetDateToday (Transaction *);

void          xaccTransSetNum (Transaction *, const char *);
void          xaccTransSetDescription (Transaction *, const char *);
void          xaccTransSetMemo (Transaction *, const char *);
void          xaccTransSetAction (Transaction *, const char *);
void          xaccTransSetReconcile (Transaction *, char);

/* return pointer to the source split */
Split *       xaccTransGetSourceSplit (Transaction *);

void          xaccSplitSetMemo (Split *, const char *);
void          xaccSplitSetAction (Split *, const char *);
void          xaccSplitSetReconcile (Split *, char);

/* freeTransaction only does so if the transaction is not part of an
 * account. (i.e. if none of the member splits are in an account). */
void          xaccFreeTransaction (Transaction *);

void xaccTransAppendSplit (Transaction *, Split *);
void xaccTransRemoveSplit (Transaction *, Split *);

/* recompute the total transaction value, based
 * on the sum of the debit splits that belong to this
 * transaction. */
void xaccTransRecomputeAmount (Transaction *);

/* The following two functions set the amount on the split */
void         xaccSetAmount (Split *, double);
void         xaccSetShareAmount (Split *, double);


/* The following four subroutines return the running balance up
 * to & including the indicated split.
 * 
 * The balance is the currency-denominated balance.  For accounts
 * with non-unit share prices, it is correctly adjusted for
 * share prices.
 * 
 * The share-balance is the number of shares. 
 * Price fluctuations do not change the share balance.
 * 
 * The cleared-balance is the currency-denominated balance 
 * of all transactions that have been marked as cleared or reconciled.
 * It is correctly adjusted for price fluctuations.
 * 
 * The reconciled-balance is the currency-denominated balance
 * of all transactions that have been marked as reconciled.
 */

double xaccGetBalance (Split *);
double xaccGetClearedBalance (Split *);
double xaccGetReconciledBalance (Split *);
double xaccGetShareBalance (Split *);

/********************************************************************\
 * sorting comparison function
 *
 * returns a negative value if transaction a is dated earlier than b,        
 * returns a positive value if transaction a is dated later than b,
 * returns zero if both transactions are on the same date.
 *
\********************************************************************/

int  xaccTransOrder (Transaction **ta, Transaction **tb);
int  xaccSplitOrder (Split **sa, Split **sb);

/*
 * count the number of transactions in the null-terminated array
 */
int xaccCountTransactions (Transaction **tarray);

/** GLOBALS *********************************************************/

#endif /* __XACC_TRANSACTION_H__ */
