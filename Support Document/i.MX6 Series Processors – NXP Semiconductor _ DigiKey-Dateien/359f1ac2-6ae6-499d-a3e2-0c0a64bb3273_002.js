// Copyright 2006-2016 ClickTale Ltd., US Patent Pending
// Generated on: 11/6/2016 3:50:44 AM (UTC 11/6/2016 9:50:44 AM)

window.ClickTaleGlobal = window.ClickTaleGlobal || {};
ClickTaleGlobal.scripts = ClickTaleGlobal.scripts || {};
ClickTaleGlobal.scripts.filter = ClickTaleGlobal.scripts.filter || (function () {
    var recordingThreshold = Math.random() * 100;

    return {
        isRecordingApproved: function(percentage) {
            return recordingThreshold <= percentage;
        }
    }
})();

			
// Copyright 2006-2016 ClickTale Ltd., US Patent Pending
// PID: 53369
// Generated on: 11/6/2016 3:50:44 AM (UTC 11/6/2016 9:50:44 AM)



/*browsers exclusion start*/function doOnlyWhen(toDoHandler, toCheckHandler, interval, times, failHandler) {
    if ((!toDoHandler) || (!toCheckHandler)) return;
    if (typeof interval == "undefined") interval = 1000;
    if (typeof times == "undefined") times = 20;

    if (--times < 0 && typeof failHandler === 'function') {
        failHandler();
        return;
    }
    if (toCheckHandler()) {
        toDoHandler();
        return;
    }

    setTimeout(function () { doOnlyWhen(toDoHandler, toCheckHandler, interval, times); }, interval);
}
doOnlyWhen(function () { if (window.ClickTaleSettings.PTC.okToRunPCC) { (function(){
window.ClickTaleSettings = window.ClickTaleSettings || {};
window.ClickTaleSettings.PTC = window.ClickTaleSettings.PTC || {};
window.ClickTaleSettings.PTC.originalPCCLocation = "P1_PID53369";
var d=!0,e=!1,j=this;var k,l,m,n;function o(){return j.navigator?j.navigator.userAgent:null}n=m=l=k=e;var p;if(p=o()){var q=j.navigator;k=0==p.indexOf("Opera");l=!k&&-1!=p.indexOf("MSIE");m=!k&&-1!=p.indexOf("WebKit");n=!k&&!m&&"Gecko"==q.product}var r=k,s=l,t=n,u=m,w;
a:{var x="",y;if(r&&j.opera)var z=j.opera.version,x="function"==typeof z?z():z;else if(t?y=/rv\:([^\);]+)(\)|;)/:s?y=/MSIE\s+([^\);]+)(\)|;)/:u&&(y=/WebKit\/(\S+)/),y)var A=y.exec(o()),x=A?A[1]:"";if(s){var B,C=j.document;B=C?C.documentMode:void 0;if(B>parseFloat(x)){w=""+B;break a}}w=x}var D={};
function E(a){var b;if(!(b=D[a])){b=0;for(var c=(""+w).replace(/^[\s\xa0]+|[\s\xa0]+$/g,"").split("."),f=(""+a).replace(/^[\s\xa0]+|[\s\xa0]+$/g,"").split("."),g=Math.max(c.length,f.length),v=0;0==b&&v<g;v++){var V=c[v]||"",W=f[v]||"",X=RegExp("(\\d*)(\\D*)","g"),Y=RegExp("(\\d*)(\\D*)","g");do{var i=X.exec(V)||["","",""],h=Y.exec(W)||["","",""];if(0==i[0].length&&0==h[0].length)break;b=((0==i[1].length?0:parseInt(i[1],10))<(0==h[1].length?0:parseInt(h[1],10))?-1:(0==i[1].length?0:parseInt(i[1],10))>
(0==h[1].length?0:parseInt(h[1],10))?1:0)||((0==i[2].length)<(0==h[2].length)?-1:(0==i[2].length)>(0==h[2].length)?1:0)||(i[2]<h[2]?-1:i[2]>h[2]?1:0)}while(0==b)}b=D[a]=0<=b}return b}var F={};function G(){F[9]||(F[9]=s&&!!document.documentMode&&9<=document.documentMode)};!s||G();!s||G();s&&E("8");!u||E("528");t&&E("1.9b")||s&&E("8")||r&&E("9.5")||u&&E("528");!t||E("8");function H(a,b,c,f,g){a&&b&&("undefined"==typeof c&&(c=1E3),"undefined"==typeof f&&(f=20),0>--f?"function"===typeof g&&g():b()?a():setTimeout(function(){H(a,b,c,f,g)},c))};function I(a,b){var c=Element.prototype;I=function(a,b){return I.d.call(a,b)};I.d=c.matches||c.webkitMatchesSelector||c.mozMatchesSelector||c.msMatchesSelector;return I(a,b)}function J(a,b){J=Element.prototype.closest?function(a,b){return Element.prototype.closest.call(a,b)}:function(a,b){for(;a&&!I(a,b);)a=a.parentElement;return a};return J(a,b)};function K(a){function b(){c||(c=d,a())}var c=e;"complete"===document.readyState||"interactive"===document.readyState?b():document.addEventListener&&document.addEventListener("DOMContentLoaded",b,e)}
function L(a,b,c,f,g){"string"===typeof a?(a=document.querySelectorAll(a),Array.prototype.forEach.call(a,function(a){L(a,b,c,f,g)})):a instanceof Array||a instanceof NodeList?Array.prototype.forEach.call(a,function(a){L(a,b,c,f,g)}):a.addEventListener(b,function(a,b,c,f,g){return function(i){if("function"===typeof c)c.apply(this,arguments),g&&a.removeEventListener(b,arguments.callee,e);else{var h=J(i.target,c);h&&(f.apply(h,arguments),g&&a.removeEventListener(b,arguments.callee,e))}}}(a,b,c,f,g),
e)}function aa(a,b){document.addEventListener("mouseup",function(c){a===c.target&&b();document.removeEventListener("mouseup",arguments.callee,e)},e)}function ba(a,b){function c(c){document.removeEventListener("touchend",arguments.callee,e);a===c.target&&b()}document.addEventListener("touchend",c,e);document.addEventListener("touchmove",function(a){document.removeEventListener("touchmove",arguments.callee,e);document.removeEventListener("touchend",c,e)},e)}
function M(a,b){var c=N();c&&(M=c.m?ba:aa,M(a,b))};function O(a){if(window.CSS&&"function"===typeof window.CSS.escape)O=function(a){return window.CSS.escape.call(window.CSS,a)};else{var b=/([\0-\x1f\x7f]|^-?\d)|^-$|[^\x80-\uFFFF\w-]/g,c=function(a,b){return b?"\x00"===a?"\ufffd":a.slice(0,-1)+"\\"+a.charCodeAt(a.length-1).toString(16)+" ":"\\"+a};O=function(a){return(a+"").replace(b,c)}}return O(a)};function P(a,b){"function"===typeof ClickTaleEvent&&(b?P.b[a]!==d&&(P.b[a]=d,ClickTaleEvent(a)):ClickTaleEvent(a))}P.b={};function Q(a){"function"===typeof window.ClickTaleRegisterElementAction&&(ClickTaleRegisterElementAction("mouseover",a),ClickTaleRegisterElementAction("click",a))}function ca(a,b){var c={},f;for(f in a)c[f]=a[f];c.target=b;c.srcElement=b;Q(c)}window.ClickTaleDetectAgent&&window.ClickTaleDetectAgent()&&window.ClickTaleDetectAgent();
function da(a,b){"object"==typeof a&&"string"==typeof b&&(window.ClickTaleContext&&-1!=document.referrer.indexOf(location.hostname)&&window.parent.ct&&window.parent.ct.ElementAddressing&&"function"===typeof window.parent.ct.ElementAddressing.setCustomElementID?window.parent.ct.ElementAddressing.setCustomElementID(a,b):(window.ClickTaleSetCustomElementID=window.ClickTaleSetCustomElementID||function(a,b){a.ClickTale=a.ClickTale||{};a.ClickTale.CustomID=b},window.ClickTaleSetCustomElementID(a,b)))}
function ea(){Array.prototype.forEach.call(document.querySelectorAll("[id]"),function(a){if(!I(a,'input[type="hidden"]')){var b=a.getAttribute("id");b.match(/(?:\r|\n)/)&&"function"===typeof ClickTaleNote&&ClickTaleNote("ctlib.api.SetCustomElementIdDuplicates: ids with line break found!");var a=document.querySelectorAll('[id="'+O(b)+'"]'),c=fa;1<a.length&&!c[b]&&(c[b]=d,Array.prototype.forEach.call(a,function(a,c){da(a,b.replace(/(\r|\n|\r\n|\s+)+/g,"_").replace(/\W/g,"_")+"_"+c)}))}})}var fa={};
function N(){if("function"===typeof ClickTaleDetectAgent){var a=ClickTaleDetectAgent();if(a)return N=function(){return a},N()}return null}function ga(a){if("function"===typeof ClickTaleRegisterTouchAction){var b=a.getBoundingClientRect();ClickTaleRegisterTouchAction(a,b.left+document.body.scrollLeft,b.top+document.body.scrollTop)}}
function ha(){var a;if(!a){a="mousedown";if("boolean"!=typeof R){var b=N();b&&(R=b.m)}R&&(a="touchstart")}L(document,a,"img, a, button, textarea, input, select",function(a){var b=a.target,g=this;M(b,function(a,b){return function(){if(I(this,"button,a,textarea")&&this!=a)R?ga(this):ca(b,this);else if(!R){var c=function(){};document.addEventListener("click",function(a){return c=function(b){b.target===a&&(S=d);document.removeEventListener("click",arguments.callee,e)}}(a),e);setTimeout(function(){S||
Q(b);document.removeEventListener("click",c,e);S=void 0},200)}}.bind(g)}(b,a))})}var R,S;function T(a){"function"===typeof ClickTaleExec&&ClickTaleExec(a)}var U=P;function ia(){var a=Z.toString();"function"===typeof ClickTaleField&&ClickTaleField("isMobile",a)};var ja=document.location.pathname.toLowerCase(),Z=e;
function ka(){if(!window.ClickTaleFirstPCCGo){window.ClickTaleFirstPCCGo=d;var a=N();a&&(Z=a.m,ia());ea();"/ordering/address"==ja&&jQuery("#primaryButton").click(function(){if("function"==typeof ClickTaleRegisterFormSubmit){var a=jQuery("#mainForm").get(0);ClickTaleRegisterFormSubmit(a)}});jQuery("#search-keyword,#custom-search").keyup(function(){var a=jQuery(this).val();T('jQuery("#search-keyword").val("'+a+'");')});jQuery("#header-search").keyup(function(){var a=jQuery(this).val();T('jQuery("#header-search").val("'+
a+'");')});"function"===typeof jQuery.fn.on&&(jQuery(document).on("click","#header-search-button",function(){"function"==typeof U&&U("Action | Activates Top Search")}),jQuery(document).on("click","#search-within-button",function(){U("Action | Activates Secondary Search")}),jQuery(document).on("click","#header-search-type",function(){U("Action | Opens All Product Dropdown")}));jQuery("#header-search").keyup(function(a){"13"==a.keyCode&&U("Action | Activates Top Search")});jQuery(".search-input,#custom-search").keyup(function(a){"13"==
a.keyCode&&U("Action | Activates Secondary Search")});if("/"==document.location.pathname||0<jQuery(".home").length)jQuery(".product-search-text").keyup(function(a){var c=jQuery(this).val();T('jQuery(".product-search-text").val("'+c+'");');"13"==a.keyCode&&U("Action | Homepage | Activates Search")}),jQuery("#pagelayout_0_content_2__searchImg").click(function(){U("Action | Homepage | Activates Search")}),jQuery("#pagelayout_0_content_2__productCategories").change(function(){"In Stock Products Only"==
jQuery("#pagelayout_0_content_2__productCategories>option:selected").text()&&U("Action | Homepage | Search | Selected In Stock Products in Dropdown")}),jQuery("#stockCheckbox").click(function(){U("Action | Homepage | Search | Clicked In Stock")}),jQuery("#rohsCheckbox").click(function(){U("Action | Homepage | Search | Clicked RoHS")}),jQuery("#leadFreeCheckbox").click(function(){U("Action | Homepage | Search | Clicked Lead Free")});-1<document.location.href.indexOf("product-search")&&0<jQuery("#topResults").length&&
U("Onload | Search Results Page | With Results");-1<document.location.href.indexOf("product-search")&&0<jQuery("#noResultsTable").length&&U("Onload | Search Results Page | Without Results");-1<document.location.href.indexOf("product-detail")&&0<jQuery("#pdp_content").length&&(U("Onload | Product Detail Page"),jQuery("#addtoorderbutton").click(function(){U("Action | Clicked Add to Cart")}));-1<document.location.href.indexOf("Ordering/AddPart.aspx")&&U("Onload | Cart Page");-1<document.location.href.indexOf("Submit.aspx?submit=yes")&&
U("Onload | Order Confirmation")}}(function(a){function b(){2==++window.okToStartOn2&&a()}window.okToStartOn2=0;K(function(){b()});if("function"==typeof ClickTaleIsRecording&&ClickTaleIsRecording()===d)b();else{var c=window.ClickTaleOnRecording||function(){};window.ClickTaleOnRecording=function(){b();return c.apply(this,arguments)}}})(function(){ha();H(ka,function(){return window.jQuery?d:e},250,40)});})();} }, function () { return !!(window.ClickTaleSettings && window.ClickTaleSettings.PTC && typeof window.ClickTaleSettings.PTC.okToRunPCC != 'undefined'); }, 500, 20);
