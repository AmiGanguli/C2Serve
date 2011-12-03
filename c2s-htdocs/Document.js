/*************************************************************************
  This code is from Dynamic Web Coding at http://www.dyn-web.com/
  See Terms of Use at http://www.dyn-web.com/bus/terms.html
  regarding conditions under which you may use this code.
  This notice must be retained in the code as is!
*************************************************************************/

/**
 * @brief   Adjust the size of the iframe element depending its the content
 * @param   iframeWindow  The iframe element.
 */
function adjustIFrameSize ( iframeWindow )
{
  var iframeEl = document.getElementById ? document.getElementById( iframeWindow.name ) : document.all ? document.all[ iframeWindow.name ] : null;
  
  if ( iframeEl != null && iframeWindow != null )
  {
    iframeEl.style.height = "auto"; // helps resize (for some) if new doc shorter than previous  
    var docHt = getDocumentHeight(iframeWindow.document);
    
    if (docHt) iframeEl.style.height = docHt /*+ 30*/ + "px";
  }
}

/**
 * @brief   Get the height of the documents content
 * @param   doc The document element
 * @return  The height of the document element.
 */
function getDocumentHeight( doc )
{
  var docHt = 0, sh, oh;
  if ( doc.height )
    docHt = doc.height;
  else if ( doc.body )
  {
    if ( doc.body.scrollHeight ) docHt = sh = doc.body.scrollHeight;
    if ( doc.body.offsetHeight ) docHt = oh = doc.body.offsetHeight;
    if ( sh && oh ) docHt = Math.max( sh , oh );
  }
  
  return docHt;
}



