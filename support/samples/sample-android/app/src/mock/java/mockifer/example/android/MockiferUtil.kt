package mockifer.example.android

import mockifer.Mockifer

object MockiferUtil {
    fun pushMock(mockId: String) {
        Mockifer.pushMock(mockId)
    }

    fun reset() {
        Mockifer.reset()
    }
}