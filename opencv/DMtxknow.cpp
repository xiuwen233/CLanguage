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

{
	DmtxMessage *msg;
	DmtxRegion *reg;
	Mat src = imread("E://photo_opencv/boxTest/555666.jpg");
	if (!src.data) {
		cout << "Load image failed!" << endl;
		return 0;
	}
	DmtxImage *img;
	img = dmtxImageCreate(src.data, src.cols, src.rows, DmtxPack24bppRGB);
	DmtxDecode *dec = dmtxDecodeCreate(img, 1);
	reg = dmtxRegionFindNext(dec, NULL);
	if (reg != NULL) {
		msg = dmtxDecodeMatrixRegion(dec, reg, DmtxUndefined);
		if (msg != NULL) {
			//cout << msg->output << endl;
			printf("msg->output = %s \n", msg->output );
		    printf(" msg->array = %x\n",msg->array);
			printf("msg->code = %x \n",msg->code);
			dmtxMessageDestroy(&msg);
		}
		dmtxRegionDestroy(&reg);
	}
	dmtxDecodeDestroy(&dec);
	dmtxImageDestroy(&img);
	getchar();
	return 0;
}
