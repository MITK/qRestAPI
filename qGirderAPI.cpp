/*==============================================================================

  Library: qRestAPI

  Copyright (c) 2021 Kitware Inc.

  See Doc/copyright/copyright.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Jean-Christophe Fillion-Robin, Kitware Inc.

==============================================================================*/

// Qt includes
#include <QUrl>
#include <QScriptEngine>
#include <QScriptValue>

// qRestAPI includes
#include "qGirderAPI.h"
#include "qRestResult.h"

// --------------------------------------------------------------------------
// qGirderAPI methods

// --------------------------------------------------------------------------
qGirderAPI::qGirderAPI(QObject* _parent)
  : Superclass(_parent)
{
}

// --------------------------------------------------------------------------
qGirderAPI::~qGirderAPI()
{
}

// --------------------------------------------------------------------------
bool qGirderAPI::parseGirderAPIv1Response(const QByteArray& response, QList<QVariantMap>& result)
{
  QScriptEngine scriptEngine;
  QScriptValue scriptValue = scriptEngine
                                .evaluate("JSON.parse")
                                .call(QScriptValue(),
                                      QScriptValueList() << QString(response));

  // e.g. {["key1": "value1", ...]} or {"key1": "value1", ...}
  if (scriptValue.isArray())
    {
    quint32 length = scriptValue.property("length").toUInt32();
    for(quint32 i = 0; i < length; ++i)
      {
      qRestAPI::appendScriptValueToVariantMapList(result, scriptValue.property(i));
      }
    }
  else
    {
    qRestAPI::appendScriptValueToVariantMapList(result, scriptValue);
    }
  return true;
}

// --------------------------------------------------------------------------
bool qGirderAPI::parseGirderAPIv1Response(qRestResult* restResult, const QByteArray& response)
{
  QList<QVariantMap> result;
  bool success = qGirderAPI::parseGirderAPIv1Response(response, result);
  restResult->setResult(result);
  return success;
}

// --------------------------------------------------------------------------
void qGirderAPI::parseResponse(qRestResult* restResult, const QByteArray& response)
{
  Q_UNUSED(response);
  qGirderAPI::parseGirderAPIv1Response(restResult, response);
}
