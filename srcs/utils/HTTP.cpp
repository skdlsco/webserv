#include "HTTP.hpp"

std::string web::getStatusMessage(int statusCode)
{
	switch (statusCode)
	{
		case 100:
			return (web::STATUS_MESSAGE_100);
		case 101:
			return (web::STATUS_MESSAGE_101);
		case 103:
			return (web::STATUS_MESSAGE_103);
		case 200:
			return (web::STATUS_MESSAGE_200);
		case 201:
			return (web::STATUS_MESSAGE_201);
		case 202:
			return (web::STATUS_MESSAGE_202);
		case 203:
			return (web::STATUS_MESSAGE_203);
		case 204:
			return (web::STATUS_MESSAGE_204);
		case 205:
			return (web::STATUS_MESSAGE_205);
		case 206:
			return (web::STATUS_MESSAGE_206);
		case 300:
			return (web::STATUS_MESSAGE_300);
		case 301:
			return (web::STATUS_MESSAGE_301);
		case 302:
			return (web::STATUS_MESSAGE_302);
		case 303:
			return (web::STATUS_MESSAGE_303);
		case 304:
			return (web::STATUS_MESSAGE_304);
		case 307:
			return (web::STATUS_MESSAGE_307);
		case 308:
			return (web::STATUS_MESSAGE_308);
		case 400:
			return (web::STATUS_MESSAGE_400);
		case 401:
			return (web::STATUS_MESSAGE_401);
		case 402:
			return (web::STATUS_MESSAGE_402);
		case 403:
			return (web::STATUS_MESSAGE_403);
		case 404:
			return (web::STATUS_MESSAGE_404);
		case 405:
			return (web::STATUS_MESSAGE_405);
		case 406:
			return (web::STATUS_MESSAGE_406);
		case 407:
			return (web::STATUS_MESSAGE_407);
		case 408:
			return (web::STATUS_MESSAGE_408);
		case 409:
			return (web::STATUS_MESSAGE_409);
		case 410:
			return (web::STATUS_MESSAGE_410);
		case 411:
			return (web::STATUS_MESSAGE_411);
		case 412:
			return (web::STATUS_MESSAGE_412);
		case 413:
			return (web::STATUS_MESSAGE_413);
		case 414:
			return (web::STATUS_MESSAGE_414);
		case 415:
			return (web::STATUS_MESSAGE_415);
		case 416:
			return (web::STATUS_MESSAGE_416);
		case 417:
			return (web::STATUS_MESSAGE_417);
		case 418:
			return (web::STATUS_MESSAGE_418);
		case 422:
			return (web::STATUS_MESSAGE_422);
		case 425:
			return (web::STATUS_MESSAGE_425);
		case 426:
			return (web::STATUS_MESSAGE_426);
		case 428:
			return (web::STATUS_MESSAGE_428);
		case 429:
			return (web::STATUS_MESSAGE_429);
		case 431:
			return (web::STATUS_MESSAGE_431);
		case 451:
			return (web::STATUS_MESSAGE_451);
		case 500:
			return (web::STATUS_MESSAGE_500);
		case 501:
			return (web::STATUS_MESSAGE_501);
		case 502:
			return (web::STATUS_MESSAGE_502);
		case 503:
			return (web::STATUS_MESSAGE_503);
		case 504:
			return (web::STATUS_MESSAGE_504);
		case 505:
			return (web::STATUS_MESSAGE_505);
		case 506:
			return (web::STATUS_MESSAGE_506);
		case 507:
			return (web::STATUS_MESSAGE_507);
		case 508:
			return (web::STATUS_MESSAGE_508);
		case 510:
			return (web::STATUS_MESSAGE_510);
		case 511:
			return (web::STATUS_MESSAGE_511);
	}
	return (web::STATUS_MESSAGE_UNDEFINED);
}


std::string web::getErrorPage(int statusCode)
{
	switch (statusCode)
	{
		case 100:
			return (web::ERROR_PAGE_100);
		case 101:
			return (web::ERROR_PAGE_101);
		case 103:
			return (web::ERROR_PAGE_103);
		case 200:
			return (web::ERROR_PAGE_200);
		case 201:
			return (web::ERROR_PAGE_201);
		case 202:
			return (web::ERROR_PAGE_202);
		case 203:
			return (web::ERROR_PAGE_203);
		case 204:
			return (web::ERROR_PAGE_204);
		case 205:
			return (web::ERROR_PAGE_205);
		case 206:
			return (web::ERROR_PAGE_206);
		case 300:
			return (web::ERROR_PAGE_300);
		case 301:
			return (web::ERROR_PAGE_301);
		case 302:
			return (web::ERROR_PAGE_302);
		case 303:
			return (web::ERROR_PAGE_303);
		case 304:
			return (web::ERROR_PAGE_304);
		case 307:
			return (web::ERROR_PAGE_307);
		case 308:
			return (web::ERROR_PAGE_308);
		case 400:
			return (web::ERROR_PAGE_400);
		case 401:
			return (web::ERROR_PAGE_401);
		case 402:
			return (web::ERROR_PAGE_402);
		case 403:
			return (web::ERROR_PAGE_403);
		case 404:
			return (web::ERROR_PAGE_404);
		case 405:
			return (web::ERROR_PAGE_405);
		case 406:
			return (web::ERROR_PAGE_406);
		case 407:
			return (web::ERROR_PAGE_407);
		case 408:
			return (web::ERROR_PAGE_408);
		case 409:
			return (web::ERROR_PAGE_409);
		case 410:
			return (web::ERROR_PAGE_410);
		case 411:
			return (web::ERROR_PAGE_411);
		case 412:
			return (web::ERROR_PAGE_412);
		case 413:
			return (web::ERROR_PAGE_413);
		case 414:
			return (web::ERROR_PAGE_414);
		case 415:
			return (web::ERROR_PAGE_415);
		case 416:
			return (web::ERROR_PAGE_416);
		case 417:
			return (web::ERROR_PAGE_417);
		case 418:
			return (web::ERROR_PAGE_418);
		case 422:
			return (web::ERROR_PAGE_422);
		case 425:
			return (web::ERROR_PAGE_425);
		case 426:
			return (web::ERROR_PAGE_426);
		case 428:
			return (web::ERROR_PAGE_428);
		case 429:
			return (web::ERROR_PAGE_429);
		case 431:
			return (web::ERROR_PAGE_431);
		case 451:
			return (web::ERROR_PAGE_451);
		case 500:
			return (web::ERROR_PAGE_500);
		case 501:
			return (web::ERROR_PAGE_501);
		case 502:
			return (web::ERROR_PAGE_502);
		case 503:
			return (web::ERROR_PAGE_503);
		case 504:
			return (web::ERROR_PAGE_504);
		case 505:
			return (web::ERROR_PAGE_505);
		case 506:
			return (web::ERROR_PAGE_506);
		case 507:
			return (web::ERROR_PAGE_507);
		case 508:
			return (web::ERROR_PAGE_508);
		case 510:
			return (web::ERROR_PAGE_510);
		case 511:
			return (web::ERROR_PAGE_511);
	}
	return (web::ERROR_PAGE_UNDEFINED);
}

std::string getMIMEType(std::string const & extension)
{
	if (extension == ".aac")
		return (web::MIMETYPE_AAC);
	else if (extension == ".abw")
		return (web::MIMETYPE_ABW);
	else if (extension == ".arc")
		return (web::MIMETYPE_ARC);
	else if (extension == ".avi")
		return (web::MIMETYPE_AVI);
	else if (extension == ".azw")
		return (web::MIMETYPE_AZW);
	else if (extension == ".bin")
		return (web::MIMETYPE_BIN);
	else if (extension == ".bz")
		return (web::MIMETYPE_BZ);
	else if (extension == ".bz2")
		return (web::MIMETYPE_BZ2);
	else if (extension == ".csh")
		return (web::MIMETYPE_CSH);
	else if (extension == ".css")
		return (web::MIMETYPE_CSS);
	else if (extension == ".csv")
		return (web::MIMETYPE_CSV);
	else if (extension == ".doc")
		return (web::MIMETYPE_DOC);
	else if (extension == ".epub")
		return (web::MIMETYPE_EPUB);
	else if (extension == ".gif")
		return (web::MIMETYPE_GIF);
	else if (extension == ".htm")
		return (web::MIMETYPE_HTM);
	else if (extension == ".html")
		return (web::MIMETYPE_HTML);
	else if (extension == ".ico")
		return (web::MIMETYPE_ICO);
	else if (extension == ".ics")
		return (web::MIMETYPE_ICS);
	else if (extension == ".jar")
		return (web::MIMETYPE_JAR);
	else if (extension == ".jpeg")
		return (web::MIMETYPE_JPEG);
	else if (extension == ".jpg")
		return (web::MIMETYPE_JPG);
	else if (extension == ".js")
		return (web::MIMETYPE_JS);
	else if (extension == ".json")
		return (web::MIMETYPE_JSON);
	else if (extension == ".mid")
		return (web::MIMETYPE_MID);
	else if (extension == ".midi")
		return (web::MIMETYPE_MIDI);
	else if (extension == ".mpeg")
		return (web::MIMETYPE_MPEG);
	else if (extension == ".mpkg")
		return (web::MIMETYPE_MPKG);
	else if (extension == ".odp")
		return (web::MIMETYPE_ODP);
	else if (extension == ".ods")
		return (web::MIMETYPE_ODS);
	else if (extension == ".odt")
		return (web::MIMETYPE_ODT);
	else if (extension == ".oga")
		return (web::MIMETYPE_OGA);
	else if (extension == ".ogv")
		return (web::MIMETYPE_OGV);
	else if (extension == ".ogx")
		return (web::MIMETYPE_OGX);
	else if (extension == ".pdf")
		return (web::MIMETYPE_PDF);
	else if (extension == ".ppt")
		return (web::MIMETYPE_PPT);
	else if (extension == ".rar")
		return (web::MIMETYPE_RAR);
	else if (extension == ".rtf")
		return (web::MIMETYPE_RTF);
	else if (extension == ".sh")
		return (web::MIMETYPE_SH);
	else if (extension == ".svg")
		return (web::MIMETYPE_SVG);
	else if (extension == ".swf")
		return (web::MIMETYPE_SWF);
	else if (extension == ".tar")
		return (web::MIMETYPE_TAR);
	else if (extension == ".tif")
		return (web::MIMETYPE_TIF);
	else if (extension == ".tiff")
		return (web::MIMETYPE_TIFF);
	else if (extension == ".ttf")
		return (web::MIMETYPE_TTF);
	else if (extension == ".vsd")
		return (web::MIMETYPE_VSD);
	else if (extension == ".wav")
		return (web::MIMETYPE_WAV);
	else if (extension == ".weba")
		return (web::MIMETYPE_WEBA);
	else if (extension == ".webm")
		return (web::MIMETYPE_WEBM);
	else if (extension == ".webp")
		return (web::MIMETYPE_WEBP);
	else if (extension == ".woff")
		return (web::MIMETYPE_WOFF);
	else if (extension == ".xhtml")
		return (web::MIMETYPE_XHTML);
	else if (extension == ".xls")
		return (web::MIMETYPE_XLS);
	else if (extension == ".xml")
		return (web::MIMETYPE_XML);
	else if (extension == ".xul")
		return (web::MIMETYPE_XUL);
	else if (extension == ".zip")
		return (web::MIMETYPE_ZIP);
	else if (extension == ".3gp")
		return (web::MIMETYPE_3GP);
	else if (extension == ".3g2")
		return (web::MIMETYPE_3G2);
	else if (extension == ".7z")
		return (web::MIMETYPE_7Z);

	return ("text/plain");
}