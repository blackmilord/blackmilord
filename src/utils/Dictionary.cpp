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
#include <QObject>
#include <QString>

namespace Dictionary
{

QString bookMetaDataLabel(MetaData metadata)
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

QString languageCode(const QString &code)
{

#define CHECK_LANG_CODE(codeStr, resultStr) if (code == codeStr) return QString(resultStr);

    CHECK_LANG_CODE("en", "English")
    CHECK_LANG_CODE("ab", "Abkhazian")
    CHECK_LANG_CODE("aa", "Afar")
    CHECK_LANG_CODE("af", "Afrikaans")
    CHECK_LANG_CODE("sq", "Albanian")
    CHECK_LANG_CODE("am", "Amharic")
    CHECK_LANG_CODE("ar", "Arabic")
    CHECK_LANG_CODE("hy", "Armenian")
    CHECK_LANG_CODE("as", "Assamese")
    CHECK_LANG_CODE("ay", "Aymara")
    CHECK_LANG_CODE("az", "Azerbaijani")
    CHECK_LANG_CODE("ba", "Bashkir")
    CHECK_LANG_CODE("eu", "Basque")
    CHECK_LANG_CODE("bn", "Bengali")
    CHECK_LANG_CODE("bh", "Bihari")
    CHECK_LANG_CODE("bi", "Bislama")
    CHECK_LANG_CODE("be", "Breton")
    CHECK_LANG_CODE("bg", "Bulgarian")
    CHECK_LANG_CODE("my", "Burmese")
    CHECK_LANG_CODE("be", "Byelorussian")
    CHECK_LANG_CODE("ca", "Catalan")
    CHECK_LANG_CODE("zh", "Chinese")
    CHECK_LANG_CODE("co", "Corsican")
    CHECK_LANG_CODE("hr", "Croatian")
    CHECK_LANG_CODE("cs", "Czech")
    CHECK_LANG_CODE("da", "Danish")
    CHECK_LANG_CODE("nl", "Dutch")
    CHECK_LANG_CODE("dz", "Dzongkha")
    CHECK_LANG_CODE("eo", "Esperanto")
    CHECK_LANG_CODE("et", "Estonian")
    CHECK_LANG_CODE("fo", "Faroese")
    CHECK_LANG_CODE("fj", "Fijian")
    CHECK_LANG_CODE("fi", "Finnish")
    CHECK_LANG_CODE("fr", "French")
    CHECK_LANG_CODE("fy", "Frisian")
    CHECK_LANG_CODE("gl", "Gallegan")
    CHECK_LANG_CODE("ka", "Georgian")
    CHECK_LANG_CODE("de", "German")
    CHECK_LANG_CODE("el", "Greek, Modern (1453-)")
    CHECK_LANG_CODE("kl", "Greenlandic")
    CHECK_LANG_CODE("gn", "Guarani")
    CHECK_LANG_CODE("gu", "Gujarati")
    CHECK_LANG_CODE("ha", "Hausa")
    CHECK_LANG_CODE("he", "Hebrew")
    CHECK_LANG_CODE("hi", "Hindi")
    CHECK_LANG_CODE("hu", "Hungarian")
    CHECK_LANG_CODE("is", "Icelandic")
    CHECK_LANG_CODE("id", "Indonesian")
    CHECK_LANG_CODE("ia", "Interlingua (International Auxiliary language Association)")
    CHECK_LANG_CODE("iu", "Inuktitut")
    CHECK_LANG_CODE("ik", "Inupiak")
    CHECK_LANG_CODE("ga", "Irish")
    CHECK_LANG_CODE("it", "Italian")
    CHECK_LANG_CODE("ja", "Japanese")
    CHECK_LANG_CODE("jv", "Javanese")
    CHECK_LANG_CODE("jw", "Javanese")
    CHECK_LANG_CODE("kn", "Kannada")
    CHECK_LANG_CODE("ks", "Kashmiri")
    CHECK_LANG_CODE("kk", "Kazakh")
    CHECK_LANG_CODE("km", "Khmer")
    CHECK_LANG_CODE("rw", "Kinyarwanda")
    CHECK_LANG_CODE("ky", "Kirghiz")
    CHECK_LANG_CODE("ko", "Korean")
    CHECK_LANG_CODE("ku", "Kurdish")
    CHECK_LANG_CODE("oc", "Langue d'Oc (post 1500)")
    CHECK_LANG_CODE("lo", "Lao")
    CHECK_LANG_CODE("la", "Latin")
    CHECK_LANG_CODE("lv", "Latvian")
    CHECK_LANG_CODE("ln", "Lingala")
    CHECK_LANG_CODE("lt", "Lithuanian")
    CHECK_LANG_CODE("mk", "Macedonian")
    CHECK_LANG_CODE("mg", "Malagasy")
    CHECK_LANG_CODE("ms", "Malay")
    CHECK_LANG_CODE("ml", "Maltese")
    CHECK_LANG_CODE("mi", "Maori")
    CHECK_LANG_CODE("mr", "Marathi")
    CHECK_LANG_CODE("mo", "Moldavian")
    CHECK_LANG_CODE("mn", "Mongolian")
    CHECK_LANG_CODE("na", "Nauru")
    CHECK_LANG_CODE("ne", "Nepali")
    CHECK_LANG_CODE("no", "Norwegian")
    CHECK_LANG_CODE("or", "Oriya")
    CHECK_LANG_CODE("om", "Oromo")
    CHECK_LANG_CODE("pa", "Panjabi")
    CHECK_LANG_CODE("fa", "Persian")
    CHECK_LANG_CODE("pl", "Polish")
    CHECK_LANG_CODE("pt", "Portuguese")
    CHECK_LANG_CODE("ps", "Pushto")
    CHECK_LANG_CODE("qu", "Quechua")
    CHECK_LANG_CODE("rm", "Rhaeto-Romance")
    CHECK_LANG_CODE("ro", "Romanian")
    CHECK_LANG_CODE("rn", "Rundi")
    CHECK_LANG_CODE("ru", "Russian")
    CHECK_LANG_CODE("sm", "Samoan")
    CHECK_LANG_CODE("sg", "Sango")
    CHECK_LANG_CODE("sa", "Sanskrit")
    CHECK_LANG_CODE("sr", "Serbian")
    CHECK_LANG_CODE("sh", "Serbo-Croatian")
    CHECK_LANG_CODE("sn", "Shona")
    CHECK_LANG_CODE("sd", "Sindhi")
    CHECK_LANG_CODE("si", "Singhalese")
    CHECK_LANG_CODE("ss", "Siswant")
    CHECK_LANG_CODE("sk", "Slovak")
    CHECK_LANG_CODE("sl", "Slovenian")
    CHECK_LANG_CODE("so", "Somali")
    CHECK_LANG_CODE("st", "Sotho, Southern")
    CHECK_LANG_CODE("es", "Spanish")
    CHECK_LANG_CODE("su", "Sudanese")
    CHECK_LANG_CODE("sw", "Swahili")
    CHECK_LANG_CODE("sv", "Swedish")
    CHECK_LANG_CODE("tl", "Tagalog")
    CHECK_LANG_CODE("tg", "Tajik")
    CHECK_LANG_CODE("ta", "Tamil")
    CHECK_LANG_CODE("tt", "Tatar")
    CHECK_LANG_CODE("te", "Telugu")
    CHECK_LANG_CODE("th", "Thai")
    CHECK_LANG_CODE("bo", "Tibetan")
    CHECK_LANG_CODE("ti", "Tigrinya")
    CHECK_LANG_CODE("to", "Tonga (Nyasa)")
    CHECK_LANG_CODE("ts", "Tsonga")
    CHECK_LANG_CODE("tn", "Tswana")
    CHECK_LANG_CODE("tr", "Turkish")
    CHECK_LANG_CODE("tk", "Turkmen")
    CHECK_LANG_CODE("tw", "Twi")
    CHECK_LANG_CODE("ug", "Uighur")
    CHECK_LANG_CODE("uk", "Ukrainian")
    CHECK_LANG_CODE("ur", "Urdu")
    CHECK_LANG_CODE("uz", "Uzbek")
    CHECK_LANG_CODE("vi", "Vietnamese")
    CHECK_LANG_CODE("vo", "Volapük")
    CHECK_LANG_CODE("cy", "Welsh")
    CHECK_LANG_CODE("wo", "Wolof")
    CHECK_LANG_CODE("xh", "Xhosa")
    CHECK_LANG_CODE("yi", "Yiddish")
    CHECK_LANG_CODE("yo", "Yoruba")
    CHECK_LANG_CODE("za", "Zhuang")
    CHECK_LANG_CODE("zu", "Zulu")

#undef CHECK_LANG_CODE

    return "Unknown";
}

}