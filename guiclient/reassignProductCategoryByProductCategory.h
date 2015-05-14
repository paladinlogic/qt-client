/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2014 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#ifndef REASSIGNPRODUCTCATEGORYBYPRODUCTCATEGORY_H
#define REASSIGNPRODUCTCATEGORYBYPRODUCTCATEGORY_H

#include "guiclient.h"
#include "xdialog.h"
#include "ui_reassignProductCategoryByProductCategory.h"

class reassignProductCategoryByProductCategory : public XDialog, public Ui::reassignProductCategoryByProductCategory
{
    Q_OBJECT

public:
    reassignProductCategoryByProductCategory(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~reassignProductCategoryByProductCategory();

public slots:
    virtual void sReassign();

protected slots:
    virtual void languageChange();

};

#endif // REASSIGNPRODUCTCATEGORYBYPRODUCTCATEGORY_H
