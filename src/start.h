#ifndef _DISCDUMPER_H_
#define _DISCDUMPER_H_

#ifdef __cplusplus
extern "C" {
#endif

void StartFunction(void);
void Reset(void);

void SetServerIp(u32 ip);
u32 GetServerIp(void);

#ifdef __cplusplus
}
#endif

#endif
