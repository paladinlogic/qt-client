/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2010 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "assignLotSerial.h"

#include <QCloseEvent>
#include <QMessageBox>
#include <QSqlError>
#include <QVariant>

#include "createLotSerial.h"

assignLotSerial::assignLotSerial(QWidget* parent, const char* name, bool modal, Qt::WFlags fl)
    : XDialog(parent, name, modal, fl)
{
  setupUi(this);

  connect(_new, SIGNAL(clicked()), this, SLOT(sNew()));
  connect(_delete, SIGNAL(clicked()), this, SLOT(sDelete()));
  connect(_assign, SIGNAL(clicked()), this, SLOT(sAssign()));
  connect(_cancel, SIGNAL(clicked()), this, SLOT(sCancel()));
  connect(this, SIGNAL(rejected()), this, SLOT(sCancel()));

  _trapClose = TRUE;

  _item->setReadOnly(TRUE);

  _itemlocdist->addColumn( tr("Lot/Serial #"),     -1, Qt::AlignLeft,  true, "ls_number");
  _itemlocdist->addColumn( tr("Expires"), _dateColumn, Qt::AlignCenter,true, "itemlocdist_expiration");
  _itemlocdist->addColumn( tr("Warranty"),_dateColumn, Qt::AlignCenter,true, "itemlocdist_warranty");
  _itemlocdist->addColumn( tr("Qty."),     _qtyColumn, Qt::AlignRight, true, "itemlocdist_qty");

  _qtyToAssign->setPrecision(omfgThis->qtyVal());
  _qtyAssigned->setPrecision(omfgThis->qtyVal());
  _qtyBalance->setPrecision(omfgThis->qtyVal());
}

assignLotSerial::~assignLotSerial()
{
  // no need to delete child widgets, Qt does it all for us
}

void assignLotSerial::languageChange()
{
  retranslateUi(this);
}

enum SetResponse assignLotSerial::set(const ParameterList &pParams)
{
  XDialog::set(pParams);
  QVariant param;
  bool     valid;

  param = pParams.value("itemlocdist_id", &valid);
  if (valid)
  {
    q.exec("SELECT NEXTVAL('itemloc_series_seq') AS _itemloc_series;");
    if (q.first())
    {
      _itemlocdistid = param.toInt();
      _itemlocSeries = q.value("_itemloc_series").toInt();

      q.prepare( "SELECT itemlocdist_itemsite_id "
                 "FROM itemlocdist "
                 "WHERE (itemlocdist_id=:itemlocdist_id);" );
      q.bindValue(":itemlocdist_id", _itemlocdistid);
      q.exec();
      if (q.first())
        _item->setItemsiteid(q.value("itemlocdist_itemsite_id").toInt());
      else if (q.lastError().type() != QSqlError::NoError)
      {
	systemError(this, q.lastError().databaseText(), __FILE__, __LINE__);
	return UndefinedError;
      }

      sFillList();
        sNew();
    }
    else if (q.lastError().type() != QSqlError::NoError)
    {
      systemError(this, q.lastError().databaseText(), __FILE__, __LINE__);
      return UndefinedError;
    }
  }

  return NoError;
}

void assignLotSerial::closeEvent(QCloseEvent *pEvent)
{
  if (_trapClose)
  {
    QMessageBox::critical( this, tr("Cannot Cancel Distribution"),
                           tr("<p>You must indicate the Lot/Serial # to "
			      "assign and select the 'Assign' button. You may "
			      "not cancel this action." ) );
    pEvent->ignore();
  }
  else
    pEvent->accept();
}

void assignLotSerial::sNew()
{
  ParameterList params;
  params.append("itemloc_series", _itemlocSeries);
  params.append("itemlocdist_id", _itemlocdistid);
  params.append("qtyRemaining",    _qtyBalance->text());

  createLotSerial newdlg(this, "", TRUE);
  newdlg.set(params);
  if (newdlg.exec() != XDialog::Rejected)
  {
    sFillList();
    if (_qtyBalance->toDouble() > 0)
      sNew();
  }
}

void assignLotSerial::sDelete()
{
  q.prepare( "DELETE FROM itemlocdist "
             "WHERE (itemlocdist_id=:itemlocdist_id);"

             "DELETE FROM lsdetail "
             "WHERE ( (lsdetail_source_type='I')"
             " AND (lsdetail_source_id=:itemlocdist_id) );" );
  q.bindValue(":itemlocdist_id", _itemlocdist->id());
  q.exec();
  if (q.lastError().type() != QSqlError::NoError)
  {
    systemError(this, q.lastError().databaseText(), __FILE__, __LINE__);
    return;
  }

  sFillList();
}

void assignLotSerial::sClose()
{
  q.prepare( "DELETE FROM itemlocdist "
             "WHERE ( (itemlocdist_source_type='D')"
             " AND (itemlocdist_source_id=:source_id) );" );
  q.bindValue(":source_id", _itemlocdistid);
  q.exec();
  if (q.lastError().type() != QSqlError::NoError)
  {
    systemError(this, q.lastError().databaseText(), __FILE__, __LINE__);
    return;
  }

  reject();
}

void assignLotSerial::sAssign()
{
  if (_qtyBalance->toDouble() != 0.0)
  {
    QMessageBox::warning( this, tr("Incomplete Assignment"),
                          tr( "<p>You must assign a Lot/Serial # to the "
			      "remaining Quantity before saving this "
			      "assignment." ) );
    _new->setFocus();
    return;
  }

  q.prepare( "UPDATE itemlocdist "
             "SET itemlocdist_source_type='O' "
             "WHERE (itemlocdist_series=:itemlocdist_series);"

             "DELETE FROM itemlocdist "
             "WHERE (itemlocdist_id=:itemlocdist_id);" );
  q.bindValue(":itemlocdist_series", _itemlocSeries);
  q.bindValue(":itemlocdist_id", _itemlocdistid);
  q.exec();
  if (q.lastError().type() != QSqlError::NoError)
  {
    systemError(this, q.lastError().databaseText(), __FILE__, __LINE__);
    return;
  }

  _trapClose = FALSE;
  done(_itemlocSeries);
}

void assignLotSerial::sFillList()
{
  double openQty = 0;

  q.prepare( "SELECT itemlocdist_qty AS qty "
             "FROM itemlocdist "
             "WHERE (itemlocdist_id=:itemlocdist_id);" );
  q.bindValue(":itemlocdist_id", _itemlocdistid);
  q.exec();
  if (q.first())
  {
    openQty = q.value("qty").toDouble();
    _qtyToAssign->setDouble(openQty);
  }
  else if (q.lastError().type() != QSqlError::NoError)
  {
    systemError(this, q.lastError().databaseText(), __FILE__, __LINE__);
    return;
  }

  q.prepare( "SELECT COALESCE(SUM(itemlocdist_qty), 0) AS totalqty "
             "FROM itemlocdist "
             "WHERE (itemlocdist_series=:itemlocdist_series);" );
  q.bindValue(":itemlocdist_series", _itemlocSeries);
  q.exec();
  if (q.first())
  {
    _qtyAssigned->setDouble(q.value("totalqty").toDouble());
    _qtyBalance->setDouble(openQty - q.value("totalqty").toDouble());
  }
  else if (q.lastError().type() != QSqlError::NoError)
  {
    systemError(this, q.lastError().databaseText(), __FILE__, __LINE__);
    return;
  }


  q.prepare( "SELECT itemlocdist.*, ls_number,"
             "       CASE WHEN (NOT itemsite_perishable) THEN :na "
             "       END AS itemlocdist_expiration_qtdisplayrole, "
             "       CASE WHEN (NOT itemsite_warrpurc) THEN :na "
             "       END AS itemlocdist_warranty_qtdisplayrole, "
             "       'qty' AS itemlocdist_qty_xtnumericrole "
             "FROM itemlocdist "
             "LEFT OUTER JOIN itemsite ON (itemlocdist_itemsite_id = itemsite_id), "
             "ls "
             "WHERE (itemlocdist_series=:itemlocdist_series) "
             "AND (itemlocdist_ls_id=ls_id) "
             "ORDER BY ls_number;" );
  q.bindValue(":itemlocdist_series", _itemlocSeries);
  q.bindValue(":na", "N/A");
  q.exec();
  _itemlocdist->populate(q);
  if (q.lastError().type() != QSqlError::NoError)
  {
    systemError(this, q.lastError().databaseText(), __FILE__, __LINE__);
    return;
  }
}

void assignLotSerial::sCancel()
{
  done(-1);
}
