#include "mySocket.cpp"
#include "dic_reader.h"
#include "search_class.h"
#include "relsets.h"
#include "include\KLTagger\KLTagger.h"
#include "include\HString\HCharUtil.h"
#include "include\KLTTypeMng\TagString.h"
#include <iostream>


#pragma comment(lib, "Ws2_32.lib")
#define MAX_LINE_BUF 10000

#define DIC_PATH "../#Dictionary\\"

using namespace std;

void setTagOption(TaggingOption* opt) {
	//출력형태	
	opt->bGetAllTaggedResult = false;
	opt->format.bPrintIndex = false;
	opt->format.bBestFirst = true;

	//태깅관련
	opt->bApplySpelling = true;
	opt->bConcatMultipleNoun = true;

	//랭킹방법
	opt->bGetBestPathWithRule = true;
	opt->bGetBestPathWithStatistics = true;

	//로깅 관련
	opt->bMakeSpellingLog = true;
}

int main()
{
	MySocket* mSocket = new MySocket();
	if (!mSocket->mInit()) return 0;

	searchClass *sc = new searchClass();
	sc->load("korlex.dic", "krx_linear.idx", "krx_syn.idx");

	KLTagger* pTagger = new KLTagger(DIC_PATH, DEFAULT_TOKEN_RING_SIZE, 0);
	pTagger->LoadProbabilityDic();

	TaggingOption opt(NormalTagging, pTagger->GetTagPOSMngPtr());
	setTagOption(&opt);

	while (1) {
		if (mSocket->mAcceptClient())
		{
			char messageBuffer[MAX_BUFFER] = { '\0', };
			int receiveBytes = mSocket->mReceiveMessage(messageBuffer);
			if (receiveBytes > 0)
			{
				char *input = new char[MAX_LINE_BUF];
				strcpy(input, messageBuffer);
				char output[MAX_LINE_BUF] = "";

				TagString TS;
				TS.clear();

				// 입력 데이터가 여러 문장으로 구성이 된 경우 
				// 자동으로 문장을 나눠서 분석해주기 때문에
				// while문이 필요함
				while (input)
				{
					input = pTagger->DoTagging(input, opt, &TS);
					TS.CalEatableToken();

					//cout << TS.ToString(output, MAX_LINE_BUF, opt.format, 0, TS.GetWriteableTokenSize());

					int nTokenSize = TS.GetSize();
					//Token 순회
					for (int i = 0; i < nTokenSize; i++)
					{
						Token *currentToken = TS.GetToken(i);
						//cout << currentToken->Str << endl;
						MAList *taggingResult = currentToken->Rank[0];
						//분석 결과 출력
						for (int j = 0; j < taggingResult->NumOfMorphemes; j++)
						{
							//형태소 출력
							Morpheme* morp = &taggingResult->Morphemes[j];
							char morpStr[100];
							HCharUtil::ConvertJohabToWansung(morp->str, morpStr);
							//cout << "\t" << morpStr;

							list<emotion> ret_tmp = sc->get_word_emotion(morpStr, (uint8_t)1);
							list<emotion>::iterator ret_tmp_itor;
							mSocket->mSendMessage(morpStr);
							int count = 1;
							if (0 < ret_tmp.size())
							{
								for (ret_tmp_itor = ret_tmp.begin(); ret_tmp_itor != ret_tmp.end(); ret_tmp_itor++)
								{
									bool printCheck = true;

									if (ret_tmp_itor->e_type == 0x01)
										sprintf(messageBuffer, "Count %d:  중립입니다.\n", count);
									else if (ret_tmp_itor->e_type == 0x02)
										sprintf(messageBuffer, "Count %d:  (Intencity:%x) 의 긍정입니다.\n", count, ret_tmp_itor->intensity);
									else if (ret_tmp_itor->e_type == 0x03)
										sprintf(messageBuffer, "Count %d:  (intencity:%x) 의 부정입니다.\n", count, ret_tmp_itor->intensity);
									else
										sprintf(messageBuffer, "Count %d:  해당 단어의 정보가 없습니다.\n", count, ret_tmp_itor->intensity);

									mSocket->mSendMessage(messageBuffer);
									count++;
								}
							}
							else
							{
								sprintf(messageBuffer, "Count %d : 등록된 정보가 없습니다.\n", count);
								mSocket->mSendMessage(messageBuffer);
							}



							//품사정보 출력
							char pos[100];
							//cout << '/' << opt.format.pNameMng->GetTagPOSHanName(morp->TagPOS) << endl;
						}//분석 결과 출력
					}//Token 순회

					TS.RemoveHeadToken(TS.GetWriteableTokenSize());
				}

				/*list<emotion> ret_tmp = sc->get_word_emotion(messageBuffer, (uint8_t)1);
				list<emotion>::iterator ret_tmp_itor;

				int count = 1;
				if (0 < ret_tmp.size())
				{
					for (ret_tmp_itor = ret_tmp.begin(); ret_tmp_itor != ret_tmp.end(); ret_tmp_itor++)
					{
						if (ret_tmp_itor->e_type == 0x01) 
							sprintf(messageBuffer, "Count %d:  중립입니다.\n", count);
						else if (ret_tmp_itor->e_type == 0x02) 
							sprintf(messageBuffer, "Count %d:  (Intencity:%x) 의 긍정입니다.\n", count, ret_tmp_itor->intensity);
						else if (ret_tmp_itor->e_type == 0x03)
							sprintf(messageBuffer, "Count %d:  (intencity:%x) 의 부정입니다.\n", count, ret_tmp_itor->intensity);
						else 
							sprintf(messageBuffer, "Count %d:  해당 단어의 정보가 없습니다.\n", count, ret_tmp_itor->intensity);
						
						mSocket->mSendMessage(messageBuffer);
						count++;
					}
				}
				else
				{
					sprintf(messageBuffer, "Count %d : 등록된 정보가 없습니다.\n", count);
					mSocket->mSendMessage(messageBuffer);
				}
				*/
				string endMsg = "$end\n";
				mSocket->mSendMessage(&endMsg.at(0));
			}
		}
		mSocket->mCloseSocket();
	}
	WSACleanup();
	return 0;
}