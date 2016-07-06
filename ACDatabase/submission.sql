CREATE TABLE [dbo].[submission]
(
	[sid] BIGINT NOT NULL PRIMARY KEY, 
    [tid] BIGINT NOT NULL, 
    [uid] BIGINT NOT NULL, 
    [status] TINYINT NOT NULL
)
