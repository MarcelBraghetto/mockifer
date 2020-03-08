package io.github.marcelbraghetto.mockifer.example.android

import io.github.marcelbraghetto.mockifer.Mockifer

object MockiferUtil {
    fun pushMock(mockId: String) {
        Mockifer.pushMock(mockId)
    }

    fun reset() {
        Mockifer.reset()
    }
}