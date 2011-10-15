/************************************************************************
 *                                                                      *
 * Author: Lukasz Marek <lukasz.m.luki@gmail.com>                       *
 *                                                                      *
 * This file is part of BlackMilord.                                    *
 *                                                                      *
 * BlackMilord is free software: you can redistribute it and/or modify  *
 * it under the terms of the GNU General Public License as published by *
 * the Free Software Foundation, either version 3 of the License, or    *
 * (at your option) any later version.                                  *
 *                                                                      *
 * BlackMilord is distributed in the hope that it will be useful,       *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the         *
 * GNU General Public License for more details.                         *
 *                                                                      *
 * You should have received a copy of the GNU General Public License    *
 * along with BlackMilord. If not, see http://www.gnu.org/licenses/     *
 *                                                                      *
 ************************************************************************/

#include "Dictionary.h"
#include <QStringList>
#include <QString>

QStringList Dictionary::m_wordsToSkipOnSpellCheck = initWordsToSkipOnSpellCheck();
QMap<QString, QString> Dictionary::m_languageCodes = initLanguageCodes();

bool Dictionary::skipSpellCheck(const QString &word)
{
    if (m_wordsToSkipOnSpellCheck.contains(word)) {
        return true;
    }
    bool ok = false;
    word.toInt(&ok, 10);
    return ok;
}

QString Dictionary::bookMetaDataLabel(MetaData metadata)
{
    switch(metadata) {
    case METADATA_AUTHOR:
        return QObject::tr("Author");
    case METADATA_PUBLISHER:
        return QObject::tr("Publisher");
    case METADATA_DESCRIPTION:
        return QObject::tr("Description");
    case METADATA_ISBN:
        return QObject::tr("ISBN");
    case METADATA_SUBJECT:
        return QObject::tr("Subject");
    case METADATA_VERSION:
        return QObject::tr("Version");
    case METADATA_CREATION_DATE:
        return QObject::tr("Creation date");
    case METADATA_MODIFICATION_DATE:
        return QObject::tr("Modification date");
    case METADATA_LAST_BACKUP_DATE:
        return QObject::tr("Last backup date");
    case METADATA_MODIFICATION_NUMBER:
        return QObject::tr("Modification number");
    default:
        break;
    }
    Q_ASSERT(false);
    return QString();
}

const QStringList& Dictionary::wordsToSkipOnSpellCheck()
{
    return m_wordsToSkipOnSpellCheck;
}

const QMap<QString, QString>& Dictionary::languageCodes()
{
    return m_languageCodes;
}

QStringList Dictionary::initWordsToSkipOnSpellCheck()
{
    QStringList result;
    result.push_back("\"");
    result.push_back(".");
    result.push_back(",");
    result.push_back("!");
    result.push_back("?");
    result.push_back(" ");
    result.push_back("<");
    result.push_back(">");
    return result;
}

QMap<QString, QString> Dictionary::initLanguageCodes()
{
    QMap<QString, QString> result;
    result["ab"] = QObject::tr("Abkhazian");
    result["aa"] = QObject::tr("Afar");
    result["af"] = QObject::tr("Afrikaans");
    result["sq"] = QObject::tr("Albanian");
    result["am"] = QObject::tr("Amharic");
    result["ar"] = QObject::tr("Arabic");
    result["hy"] = QObject::tr("Armenian");
    result["as"] = QObject::tr("Assamese");
    result["ay"] = QObject::tr("Aymara");
    result["az"] = QObject::tr("Azerbaijani");
    result["ba"] = QObject::tr("Bashkir");
    result["eu"] = QObject::tr("Basque");
    result["bn"] = QObject::tr("Bengali");
    result["bh"] = QObject::tr("Bihari");
    result["bi"] = QObject::tr("Bislama");
    result["be"] = QObject::tr("Breton");
    result["bg"] = QObject::tr("Bulgarian");
    result["my"] = QObject::tr("Burmese");
    result["be"] = QObject::tr("Byelorussian");
    result["ca"] = QObject::tr("Catalan");
    result["zh"] = QObject::tr("Chinese");
    result["co"] = QObject::tr("Corsican");
    result["hr"] = QObject::tr("Croatian");
    result["cs"] = QObject::tr("Czech");
    result["da"] = QObject::tr("Danish");
    result["nl"] = QObject::tr("Dutch");
    result["dz"] = QObject::tr("Dzongkha");
    result["en"] = QObject::tr("English");
    result["eo"] = QObject::tr("Esperanto");
    result["et"] = QObject::tr("Estonian");
    result["fo"] = QObject::tr("Faroese");
    result["fj"] = QObject::tr("Fijian");
    result["fi"] = QObject::tr("Finnish");
    result["fr"] = QObject::tr("French");
    result["fy"] = QObject::tr("Frisian");
    result["gl"] = QObject::tr("Gallegan");
    result["ka"] = QObject::tr("Georgian");
    result["de"] = QObject::tr("German");
    result["el"] = QObject::tr("Greek, Modern (1453-)");
    result["kl"] = QObject::tr("Greenlandic");
    result["gn"] = QObject::tr("Guarani");
    result["gu"] = QObject::tr("Gujarati");
    result["ha"] = QObject::tr("Hausa");
    result["he"] = QObject::tr("Hebrew");
    result["hi"] = QObject::tr("Hindi");
    result["hu"] = QObject::tr("Hungarian");
    result["is"] = QObject::tr("Icelandic");
    result["id"] = QObject::tr("Indonesian");
    result["ia"] = QObject::tr("Interlingua (International Auxiliary language Association)");
    result["iu"] = QObject::tr("Inuktitut");
    result["ik"] = QObject::tr("Inupiak");
    result["ga"] = QObject::tr("Irish");
    result["it"] = QObject::tr("Italian");
    result["ja"] = QObject::tr("Japanese");
    result["jv"] = QObject::tr("Javanese");
    result["jw"] = QObject::tr("Javanese");
    result["kn"] = QObject::tr("Kannada");
    result["ks"] = QObject::tr("Kashmiri");
    result["kk"] = QObject::tr("Kazakh");
    result["km"] = QObject::tr("Khmer");
    result["rw"] = QObject::tr("Kinyarwanda");
    result["ky"] = QObject::tr("Kirghiz");
    result["ko"] = QObject::tr("Korean");
    result["ku"] = QObject::tr("Kurdish");
    result["oc"] = QObject::tr("Langue d'Oc (post 1500)");
    result["lo"] = QObject::tr("Lao");
    result["la"] = QObject::tr("Latin");
    result["lv"] = QObject::tr("Latvian");
    result["ln"] = QObject::tr("Lingala");
    result["lt"] = QObject::tr("Lithuanian");
    result["mk"] = QObject::tr("Macedonian");
    result["mg"] = QObject::tr("Malagasy");
    result["ms"] = QObject::tr("Malay");
    result["ml"] = QObject::tr("Maltese");
    result["mi"] = QObject::tr("Maori");
    result["mr"] = QObject::tr("Marathi");
    result["mo"] = QObject::tr("Moldavian");
    result["mn"] = QObject::tr("Mongolian");
    result["na"] = QObject::tr("Nauru");
    result["ne"] = QObject::tr("Nepali");
    result["no"] = QObject::tr("Norwegian");
    result["or"] = QObject::tr("Oriya");
    result["om"] = QObject::tr("Oromo");
    result["pa"] = QObject::tr("Panjabi");
    result["fa"] = QObject::tr("Persian");
    result["pl"] = QObject::tr("Polish");
    result["pt"] = QObject::tr("Portuguese");
    result["ps"] = QObject::tr("Pushto");
    result["qu"] = QObject::tr("Quechua");
    result["rm"] = QObject::tr("Rhaeto-Romance");
    result["ro"] = QObject::tr("Romanian");
    result["rn"] = QObject::tr("Rundi");
    result["ru"] = QObject::tr("Russian");
    result["sm"] = QObject::tr("Samoan");
    result["sg"] = QObject::tr("Sango");
    result["sa"] = QObject::tr("Sanskrit");
    result["sr"] = QObject::tr("Serbian");
    result["sh"] = QObject::tr("Serbo-Croatian");
    result["sn"] = QObject::tr("Shona");
    result["sd"] = QObject::tr("Sindhi");
    result["si"] = QObject::tr("Singhalese");
    result["ss"] = QObject::tr("Siswant");
    result["sk"] = QObject::tr("Slovak");
    result["sl"] = QObject::tr("Slovenian");
    result["so"] = QObject::tr("Somali");
    result["st"] = QObject::tr("Sotho, Southern");
    result["es"] = QObject::tr("Spanish");
    result["su"] = QObject::tr("Sudanese");
    result["sw"] = QObject::tr("Swahili");
    result["sv"] = QObject::tr("Swedish");
    result["tl"] = QObject::tr("Tagalog");
    result["tg"] = QObject::tr("Tajik");
    result["ta"] = QObject::tr("Tamil");
    result["tt"] = QObject::tr("Tatar");
    result["te"] = QObject::tr("Telugu");
    result["th"] = QObject::tr("Thai");
    result["bo"] = QObject::tr("Tibetan");
    result["ti"] = QObject::tr("Tigrinya");
    result["to"] = QObject::tr("Tonga (Nyasa)");
    result["ts"] = QObject::tr("Tsonga");
    result["tn"] = QObject::tr("Tswana");
    result["tr"] = QObject::tr("Turkish");
    result["tk"] = QObject::tr("Turkmen");
    result["tw"] = QObject::tr("Twi");
    result["ug"] = QObject::tr("Uighur");
    result["uk"] = QObject::tr("Ukrainian");
    result["ur"] = QObject::tr("Urdu");
    result["uz"] = QObject::tr("Uzbek");
    result["vi"] = QObject::tr("Vietnamese");
    result["vo"] = QObject::tr("Volapük");
    result["cy"] = QObject::tr("Welsh");
    result["wo"] = QObject::tr("Wolof");
    result["xh"] = QObject::tr("Xhosa");
    result["yi"] = QObject::tr("Yiddish");
    result["yo"] = QObject::tr("Yoruba");
    result["za"] = QObject::tr("Zhuang");
    result["zu"] = QObject::tr("Zulu");
    return result;
}
