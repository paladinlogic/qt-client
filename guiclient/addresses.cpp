/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2014 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "addresses.h"

#include <QAction>
#include <QMenu>
#include <QMessageBox>
#include <QSqlError>
#include <QVariant>

#include <openreports.h>
#include <metasql.h>

#include "addresses.h"
#include "address.h"
#include "storedProcErrorLookup.h"
#include "parameterwidget.h"
#include "errorReporter.h"
#include "prospect.h"

addresses::addresses(QWidget* parent, const char*, Qt::WindowFlags fl)
  : display(parent, "addresses", fl)
{
  setReportName("AddressesMasterList");
  setWindowTitle(tr("Addresses"));
  setMetaSQLOptions("addresses", "detail");
  setNewVisible(true);
  setQueryOnStartEnabled(true);
  setParameterWidgetVisible(true);

  parameterWidget()->append(tr("Show Inactive"), "showInactive", ParameterWidget::Exists);

  list()->addColumn(tr("Line 1"),	 -1, Qt::AlignLeft, true, "addr_line1");
  list()->addColumn(tr("Line 2"),	 75, Qt::AlignLeft, true, "addr_line2");
  list()->addColumn(tr("Line 3"),	 75, Qt::AlignLeft, true, "addr_line3");
  list()->addColumn(tr("City"),	 75, Qt::AlignLeft, true, "addr_city");
  list()->addColumn(tr("State"),	 50, Qt::AlignLeft, true, "addr_state");
  list()->addColumn(tr("Country"),	 50, Qt::AlignLeft, true, "addr_country");
  list()->addColumn(tr("Postal Code"),50,Qt::AlignLeft, true, "addr_postalcode");

  setupCharacteristics("ADDR");

  if (_privileges->check("MaintainAddresses"))
    connect(list(), SIGNAL(itemSelected(int)), this, SLOT(sEdit()));
  else
  {
    newAction()->setEnabled(false);
    connect(list(), SIGNAL(itemSelected(int)), this, SLOT(sView()));
  }

  _crmacctid = -1;
}

void addresses::sPopulateMenu(QMenu *pMenu, QTreeWidgetItem*, int)
{
  QAction *menuItem;

  menuItem = pMenu->addAction(tr("Edit..."), this, SLOT(sEdit()));
  menuItem->setEnabled(_privileges->check("MaintainAddresses"));

  menuItem = pMenu->addAction(tr("View..."), this, SLOT(sView()));
  menuItem->setEnabled(_privileges->check("ViewAddresses"));

  menuItem = pMenu->addAction(tr("Delete"), this, SLOT(sDelete()));
  menuItem->setEnabled(_privileges->check("MaintainAddresses"));

  // Create, Edit, View Prospect:
  // Temporary sql hack to determine crmacct. Replace this post crm enhancements project.
  XSqlQuery sql;
  sql.prepare("SELECT crmacct_id, crmacct_prospect_id"
              "  FROM cntct"
              "  JOIN crmacct ON cntct_crmacct_id=crmacct_id"
              " WHERE cntct_addr_id=:addr_id"
              "   AND crmacct_cust_id IS NULL;");
  sql.bindValue(":addr_id", list()->currentItem()->id());
  sql.exec();

  if (sql.first())
  {
    _crmacctid = sql.value("crmacct_id").toInt();
    bool editProspectPriv = _privileges->check("MaintainAllCRMAccounts"); // TODO - replace if a new "ViewProspect" priv created
    bool viewProspectPriv = _privileges->check("ViewAllCRMAccounts"); // TODO - replace if a new "ViewProspect" priv created
    
    if (sql.value("crmacct_prospect_id").toInt() > 0)
    {
      pMenu->addSeparator();
      menuItem = pMenu->addAction(tr("Edit Prospect"), this, SLOT(sEditProspect()));
      menuItem->setEnabled(editProspectPriv);
      menuItem = pMenu->addAction(tr("View Prospect"), this, SLOT(sViewProspect()));
      menuItem->setEnabled(viewProspectPriv);
    }
    else
    {
      pMenu->addSeparator();
      menuItem = pMenu->addAction(tr("Create Prospect..."), this, SLOT(sNewProspect()));
      menuItem->setEnabled(editProspectPriv);
    }
  }
}

void addresses::sNew()
{
  ParameterList params;
  params.append("mode", "new");

  address newdlg(this, "", true);
  newdlg.set(params);

  if (newdlg.exec() != XDialog::Rejected)
    sFillList();
}

void addresses::sEdit()
{
  ParameterList params;
  params.append("mode", "edit");
  params.append("addr_id", list()->id());

  address newdlg(this, "", true);
  newdlg.set(params);

  if (newdlg.exec() != XDialog::Rejected)
    sFillList();
}

void addresses::sView()
{
  ParameterList params;
  params.append("mode", "view");
  params.append("addr_id", list()->id());

  address newdlg(this, "", true);
  newdlg.set(params);
  newdlg.exec();
}

void addresses::sDelete()
{
  XSqlQuery deleteAddress;
  deleteAddress.prepare("SELECT deleteAddress(:addr_id) AS result;");
  deleteAddress.bindValue(":addr_id", list()->id());
  deleteAddress.exec();
  if (deleteAddress.first())
  {
    int result = deleteAddress.value("result").toInt();
    if (result < 0)
    {
      QMessageBox::warning(this, tr("Cannot Delete Selected Address"),
			   storedProcErrorLookup("deleteAddress", result));
      return;
    }
    else
      sFillList();
  }
  else if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Deleting Address"),
                                deleteAddress, __FILE__, __LINE__))
  {
    return;
  }
}

void addresses::sNewProspect()
{
  QList<XTreeWidgetItem*> selected = list()->selectedItems();
  for (int i = 0; i < selected.size(); i++)
  {
    ParameterList params;
    params.append("mode", "new");
    params.append("crmacct_id", _crmacctid);

    prospect *newdlg = new prospect();
    newdlg->set(params);
    omfgThis->handleNewWindow(newdlg);
  }
  sFillList();
}

void addresses::sEditProspect()
{
  QList<XTreeWidgetItem*> selected = list()->selectedItems();
  for (int i = 0; i < selected.size(); i++)
  {
    ParameterList params;
    params.append("mode", "edit");
    params.append("prospect_id", ((XTreeWidgetItem*)(selected[i]))->rawValue("prospect"));
    
    prospect *newdlg = new prospect();
    newdlg->set(params);
    omfgThis->handleNewWindow(newdlg);
  }
}

void addresses::sViewProspect()
{
  QList<XTreeWidgetItem*> selected = list()->selectedItems();
  for (int i = 0; i < selected.size(); i++)
  {
    ParameterList params;
    params.append("mode", "view");
    params.append("prospect_id", ((XTreeWidgetItem*)(selected[i]))->rawValue("prospect"));

    prospect *newdlg = new prospect();
    newdlg->set(params);
    omfgThis->handleNewWindow(newdlg);
  }
}
