// @version d4os_io_regions_guide
// @package d4os_io_regions_guide
// @copyright Copyright wene / ssm2017 Binder (C) 2013. All rights reserved.
// @license http://www.gnu.org/licenses/gpl-2.0.html GNU/GPL, see LICENSE.php
// d4os_io_regions_guide is free software and parts of it may contain or be derived from the
// GNU General Public License or other free or open source software licenses.

string url="http://home.ssm2017.com";
integer category = 0;
// *********************************
//      STRINGS
// *********************************
// symbols
string _SYMBOL_WARNING = "⚠";
// common
string _THE_SCRIPT_WILL_STOP = "The script will stop";
// checks
string _MISSING_VAR_NAMED = "Missing var named";
// terminal
string _UPDATE_ERROR = "Update error";
// http errors
string _HTTP_ERROR = "http error";
string _REQUEST_TIMED_OUT = "Request timed out";
string _FORBIDDEN_ACCESS = "Forbidden access";
string _PAGE_NOT_FOUND = "Page not found";
string _INTERNET_EXPLODED = "the internet exploded!!";
string _SERVER_ERROR = "Server error";
// ============================================================
//      NOTHING SHOULD BE MODIFIED UNDER THIS LINE
// ============================================================
string ARGS_SEPARATOR = "||";
integer actual_region = 0;
// *********************
//      FUNCTIONS
// *********************
// call
key get_region_id;
getRegion(string way) {
    // sending values
    get_region_id = llHTTPRequest( url+"/metaverse-framework", [HTTP_METHOD, "POST", HTTP_MIMETYPE, "application/x-www-form-urlencoded"],
                    "app=d4os_ui_regions_guide"
                    +"&cmd=get_region"
                    +"&output_type=message"
                    +"&args_separator="+ARGS_SEPARATOR
                    +"&arg="
                    +"category="+(string)category+ARGS_SEPARATOR
                    +"id="+(string)actual_region+ARGS_SEPARATOR
                    +"way="+way
                    );
}
// get server answer
getServerAnswer(integer status, string body) {
    if (status == 499) {
        llOwnerSay(_SYMBOL_WARNING+ " "+ (string)status+ " "+ _REQUEST_TIMED_OUT);
    }
    else if (status == 403) {
        llOwnerSay(_SYMBOL_WARNING+ " "+ (string)status+ " "+ _FORBIDDEN_ACCESS);
    }
    else if (status == 404) {
        llOwnerSay(_SYMBOL_WARNING+ " "+ (string)status+ " "+ _PAGE_NOT_FOUND);
    }
    else if (status == 500) {
        llOwnerSay(_SYMBOL_WARNING+ " "+ (string)status+ " "+ _SERVER_ERROR);
    }
    else if (status != 403 && status != 404 && status != 500) {
        llOwnerSay(_SYMBOL_WARNING+ " "+ (string)status+ " "+ _INTERNET_EXPLODED);
        llOwnerSay(body);
    }
}
// display result
displayResult(string data) {
    // data values : nid||region name||region texture||landing point||landing rotation||offset||total||online
    list values = llParseString2List(data, [ARGS_SEPARATOR],[]);
    // set the acual region
    actual_region = llList2Integer(values, 0);
    string region_name = llList2String(values, 1);
    key texture = llList2Key(values, 2);
    vector landing_coordinates = (vector)llUnescapeURL(llList2String(values, 3));
    vector landing_rotation = (vector)llUnescapeURL(llList2String(values, 4));
    integer offset = llList2Integer(values, 5);
    integer total = llList2Integer(values, 6);
    integer online = llList2Integer(values, 7);
    // debug values just for example
    llOwnerSay("actual_region = "+(string)actual_region);
    llOwnerSay("region_name = " +region_name);
    llOwnerSay("texture = "+(string)texture);
    llOwnerSay("landing_coordinates = "+(string)landing_coordinates);
    llOwnerSay("landing_rotation = "+(string)landing_rotation);
    llOwnerSay("offset = "+(string)offset);
    llOwnerSay("total = "+(string)total);
    llOwnerSay("online = "+(string)online);

}
// ***********************
//  INIT PROGRAM
// ***********************
default {

    state_entry() {
        integer check = 1;
        if ( url == "" ) {
            llOwnerSay(_SYMBOL_WARNING+ " "+ _MISSING_VAR_NAMED+ " \"url\"");
            check = 0;
        }
        if (!check) {
            state idle;
        }
        else {
            getRegion("start");
        }
    }

    touch_start(integer num_detected) {
        string object_name = llGetLinkName(llDetectedLinkNumber(0));
        if (object_name == "next") {
            getRegion("next");
        }
        else if (object_name == "prev") {
            getRegion("prev");
        }
    }
    http_response(key request_id, integer status, list metadata, string body) {
        if ( status != 200 ) {
            getServerAnswer(status, body);
        }
        else {
            body = llStringTrim( body , STRING_TRIM);
            list data = llParseString2List(body, [";"],[]);
            string result = llList2String(data,0);
            if ( result == "success" ) {
                displayResult(llList2String(data,1));
            }
            else {
                llOwnerSay(body);
            }
        }
    }
}

// **************
//      Error
// **************
state idle {
    touch_start(integer num_detected) {
        llResetScript();
    }
}
