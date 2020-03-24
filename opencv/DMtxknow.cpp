DmtxMessage *msg;
	DmtxRegion *reg;
	DmtxImage *img;
	DmtxDecode *dec;
  
  img = dmtxImageCreate(out.data, out.cols, out.rows, DmtxPack24bppRGB);
				dec = dmtxDecodeCreate(img, 1);
				reg = dmtxRegionFindNext(dec, NULL);
				if (reg != NULL) {
					msg = dmtxDecodeMatrixRegion(dec, reg, DmtxUndefined);
					if (msg != NULL) {
						//pcodedata = msg->output;
						//get_DataCode = false;
						printf("%s \n", msg->output);
					}
					else {
						//is_detectcode = false;
						//printf("%s \n", msg->output);
						printf(" donot have \n");
					}
				}
				else {
					//is_detectcode = false;
				}
